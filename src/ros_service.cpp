#include "orbbec_camera/ob_camera_node.h"

namespace orbbec_camera {

void OBCameraNode::setupCameraCtrlServices() {
  using std_srvs::SetBool;
  for (const auto& stream_index : IMAGE_STREAMS) {
    if (!enable_[stream_index]) {
      ROS_INFO_STREAM("Stream " << stream_name_[stream_index] << " is disabled.");
      continue;
    }
    auto stream_name = stream_name_[stream_index];
    std::string service_name = "/" + camera_name_ + "/" + "get_" + stream_name + "_exposure";
    get_exposure_srv_[stream_index] = nh_.advertiseService<GetInt32Request, GetInt32Response>(
        service_name, [this, stream_index](GetInt32Request& request, GetInt32Response& response) {
          response.success = this->getExposureCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_exposure";
    set_exposure_srv_[stream_index] = nh_.advertiseService<SetInt32Request, SetInt32Response>(
        service_name, [this, stream_index](SetInt32Request& request, SetInt32Response& response) {
          response.success = this->setExposureCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "get_" + stream_name + "_gain";
    get_gain_srv_[stream_index] = nh_.advertiseService<GetInt32Request, GetInt32Response>(
        service_name, [this, stream_index](GetInt32Request& request, GetInt32Response& response) {
          response.success = this->getGainCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_gain";
    set_gain_srv_[stream_index] = nh_.advertiseService<SetInt32Request, SetInt32Response>(
        service_name, [this, stream_index](SetInt32Request& request, SetInt32Response& response) {
          response.success = this->setGainCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_mirror";
    set_mirror_srv_[stream_index] =
        nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
            service_name, [this, stream_index](std_srvs::SetBoolRequest& request,
                                               std_srvs::SetBoolResponse& response) {
              response.success = this->setMirrorCallback(request, response, stream_index);
              return response.success;
            });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_auto_exposure";
    set_auto_exposure_srv_[stream_index] =
        nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
            service_name, [this, stream_index](std_srvs::SetBoolRequest& request,
                                               std_srvs::SetBoolResponse& response) {
              response.success = this->setAutoExposureCallback(request, response, stream_index);
              return response.success;
            });
    service_name = "/" + camera_name_ + "/" + "get_" + stream_name + "_auto_exposure";
    get_auto_exposure_srv_[stream_index] = nh_.advertiseService<GetBoolRequest, GetBoolResponse>(
        service_name, [this, stream_index](GetBoolRequest& request, GetBoolResponse& response) {
          response.success = this->getAutoExposureCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "toggle_" + stream_name;
    toggle_sensor_srv_[stream_index] =
        nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
            service_name, [this, stream_index = stream_index](auto&& request, auto&& response) {
              response.success = this->toggleSensorCallback(request, response, stream_index);
              return response.success;
            });
    service_name = "/" + camera_name_ + "/" + "get_" + stream_name + "_camera_info";
    get_camera_info_srv_[stream_index] =
        nh_.advertiseService<GetCameraInfoRequest, GetCameraInfoResponse>(
            service_name,
            [this, stream_index](GetCameraInfoRequest& request, GetCameraInfoResponse& response) {
              response.success = this->getCameraInfoCallback(request, response, stream_index);
              return response.success;
            });
    service_name = "/" + camera_name_ + "/" + "save_" + stream_name + "_image";
    save_images_srv_[stream_index] =
        nh_.advertiseService<std_srvs::EmptyRequest, std_srvs::EmptyResponse>(
            service_name, [this, stream_index](std_srvs::EmptyRequest& request,
                                               std_srvs::EmptyResponse& response) {
              return this->saveImagesCallback(request, response);
            });
  }
  get_auto_white_balance_srv_ = nh_.advertiseService<GetInt32Request, GetInt32Response>(
      "/" + camera_name_ + "/" + "get_auto_white_balance",
      [this](GetInt32Request& request, GetInt32Response& response) {
        response.success = this->getAutoWhiteBalanceCallback(request, response);
        return response.success;
      });
  set_auto_white_balance_srv_ = nh_.advertiseService<SetInt32Request, SetInt32Response>(
      "/" + camera_name_ + "/" + "set_auto_white_balance",
      [this](SetInt32Request& request, SetInt32Response& response) {
        response.success = this->setAutoWhiteBalanceCallback(request, response);
        return response.success;
      });
  get_white_balance_srv_ = nh_.advertiseService<GetInt32Request, GetInt32Response>(
      "/" + camera_name_ + "/" + "get_white_balance",
      [this](GetInt32Request& request, GetInt32Response& response) {
        response.success = this->getWhiteBalanceCallback(request, response);
        return response.success;
      });
  set_white_balance_srv_ = nh_.advertiseService<SetInt32Request, SetInt32Response>(
      "/" + camera_name_ + "/" + "set_white_balance",
      [this](SetInt32Request& request, SetInt32Response& response) {
        response.success = this->setWhiteBalanceCallback(request, response);
        return response.success;
      });
  set_fan_srv_ = nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
      "/" + camera_name_ + "/" + "set_fan",
      [this](std_srvs::SetBoolRequest& request, std_srvs::SetBoolResponse& response) {
        response.success = this->setFanCallback(request, response);
        return response.success;
      });
  set_floor_srv_ = nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
      "/" + camera_name_ + "/" + "set_floor",
      [this](std_srvs::SetBoolRequest& request, std_srvs::SetBoolResponse& response) {
        response.success = this->setFloorCallback(request, response);
        return response.success;
      });
  set_laser_srv_ = nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
      "/" + camera_name_ + "/" + "set_laser",
      [this](std_srvs::SetBoolRequest& request, std_srvs::SetBoolResponse& response) {
        response.success = this->setLaserCallback(request, response);
        return response.success;
      });
  set_ldp_srv_ = nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
      "/" + camera_name_ + "/" + "set_ldp",
      [this](std_srvs::SetBoolRequest& request, std_srvs::SetBoolResponse& response) {
        response.success = this->setLdpEnableCallback(request, response);
        return response.success;
      });
  get_device_info_srv_ = nh_.advertiseService<GetDeviceInfoRequest, GetDeviceInfoResponse>(
      "/" + camera_name_ + "/" + "get_device_info",
      [this](GetDeviceInfoRequest& request, GetDeviceInfoResponse& response) {
        response.success = this->getDeviceInfoCallback(request, response);
        return response.success;
      });
  get_serial_number_srv_ = nh_.advertiseService<GetStringRequest, GetStringResponse>(
      "/" + camera_name_ + "/" + "get_serial",
      [this](GetStringRequest& request, GetStringResponse& response) {
        response.success = this->getSerialNumberCallback(request, response);
        return response.success;
      });
  get_camera_params_srv_ = nh_.advertiseService<GetCameraParamsRequest, GetCameraParamsResponse>(
      "/" + camera_name_ + "/" + "get_camera_params",
      [this](GetCameraParamsRequest& request, GetCameraParamsResponse& response) {
        response.success = this->getCameraParamsCallback(request, response);
        return response.success;
      });

  get_sdk_version_srv_ = nh_.advertiseService<GetStringRequest, GetStringResponse>(
      "/" + camera_name_ + "/" + "get_version",
      [this](GetStringRequest& request, GetStringResponse& response) {
        response.success = this->getSDKVersionCallback(request, response);
        return response.success;
      });
  get_device_type_srv_ = nh_.advertiseService<GetStringRequest, GetStringResponse>(
      "/" + camera_name_ + "/" + "get_device_type",
      [this](GetStringRequest& request, GetStringResponse& response) {
        response.success = this->getDeviceTypeCallback(request, response);
        return response.success;
      });
  save_point_cloud_xyz_srv_ = nh_.advertiseService<std_srvs::EmptyRequest, std_srvs::EmptyResponse>(
      "/" + camera_name_ + "/" + "save_point_cloud_xyz",
      [this](std_srvs::EmptyRequest& request, std_srvs::EmptyResponse& response) {
        return this->savePointCloudXYZCallback(request, response);
      });
  save_point_cloud_xyzrgb_srv_ =
      nh_.advertiseService<std_srvs::EmptyRequest, std_srvs::EmptyResponse>(
          "/" + camera_name_ + "/" + "save_point_cloud_xyzrgb",
          [this](std_srvs::EmptyRequest& request, std_srvs::EmptyResponse& response) {
            return this->savePointCloudXYZRGBCallback(request, response);
          });
}

bool OBCameraNode::setMirrorCallback(std_srvs::SetBoolRequest& request,
                                     std_srvs::SetBoolResponse& response,
                                     const stream_index_pair& stream_index) {
  if (!enable_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return response.success;
  }
  auto sensor = sensors_[stream_index];
  try {
    sensor->setMirror(request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to set mirror mode: " << e.getMessage());
    response.success = false;
    return response.success;
  }
  return true;
}

bool OBCameraNode::getExposureCallback(GetInt32Request& request, GetInt32Response& response,
                                       const stream_index_pair& stream_index) {
  (void)request;
  if (!enable_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    response.data = sensor->getExposure();
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to get exposure: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::setExposureCallback(SetInt32Request& request, SetInt32Response& response,
                                       const stream_index_pair& stream_index) {
  if (!enable_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    sensor->setExposure(request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to set exposure: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::getGainCallback(GetInt32Request& request, GetInt32Response& response,
                                   const stream_index_pair& stream_index) {
  (void)request;
  if (!enable_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    response.data = sensor->getGain();
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to get gain: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::setGainCallback(SetInt32Request& request, SetInt32Response& response,
                                   const stream_index_pair& stream_index) {
  if (!enable_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    sensor->setGain(request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to set gain: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::getAutoWhiteBalanceCallback(GetInt32Request& request,
                                               GetInt32Response& response) {
  (void)request;
  if (!enable_[COLOR]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[COLOR] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[COLOR];
  try {
    response.data = sensor->getAutoWhiteBalance();
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to get auto white balance: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::setAutoWhiteBalanceCallback(SetInt32Request& request,
                                               SetInt32Response& response) {
  if (!enable_[COLOR]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[COLOR] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[COLOR];
  try {
    sensor->setAutoWhiteBalance(request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to set auto white balance: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::getWhiteBalanceCallback(GetInt32Request& request, GetInt32Response& response) {
  (void)request;
  if (!enable_[COLOR]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[COLOR] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[COLOR];
  try {
    response.data = sensor->getWhiteBalance();
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to get white balance: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::setWhiteBalanceCallback(SetInt32Request& request, SetInt32Response& response) {
  if (!enable_[COLOR]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[COLOR] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[COLOR];
  try {
    sensor->setWhiteBalance(request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to set white balance: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::setAutoExposureCallback(std_srvs::SetBoolRequest& request,
                                           std_srvs::SetBoolResponse& response,
                                           const stream_index_pair& stream_index) {
  if (!enable_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    sensor->setAutoExposure(request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to set auto exposure: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::getAutoExposureCallback(GetBoolRequest& request, GetBoolResponse& response,
                                           const stream_index_pair& stream_index) {
  (void)request;
  if (!enable_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    response.data = sensor->getAutoExposure();
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to get auto exposure: " << e.getMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool OBCameraNode::setLaserCallback(std_srvs::SetBoolRequest& request,
                                    std_srvs::SetBoolResponse& response) {
  (void)response;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  try {
    device_->setBoolProperty(OB_PROP_LASER_BOOL, request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to set laser: " << e.getMessage());
    response.message = e.getMessage();
    return false;
  }
  return true;
}

bool OBCameraNode::setLdpEnableCallback(std_srvs::SetBoolRequest& request,
                                        std_srvs::SetBoolResponse& response) {
  (void)response;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  try {
    device_->setBoolProperty(OB_PROP_LDP_BOOL, request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to set LDP: " << e.getMessage());
    response.message = e.getMessage();
    return false;
  }
  return true;
}

bool OBCameraNode::setFanCallback(std_srvs::SetBoolRequest& request,
                                  std_srvs::SetBoolResponse& response) {
  (void)response;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  try {
    device_->setBoolProperty(OB_PROP_FAN_WORK_MODE_INT, request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("set fan failed: " << e.getMessage());
    response.message = e.getMessage();
    return false;
  }
  return true;
}

bool OBCameraNode::setFloorCallback(std_srvs::SetBoolRequest& request,
                                    std_srvs::SetBoolResponse& response) {
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  try {
    device_->setBoolProperty(OB_PROP_FLOOD_BOOL, request.data);
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("set floor failed: " << e.getMessage());
    response.message = e.getMessage();
    return false;
  }
  return true;
}

bool OBCameraNode::getDeviceInfoCallback(GetDeviceInfoRequest& request,
                                         GetDeviceInfoResponse& response) {
  (void)request;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  auto device_info = device_->getDeviceInfo();
  response.info.name = device_info->name();
  response.info.pid = device_info->pid();
  response.info.vid = device_info->vid();
  response.info.serial_number = device_info->serialNumber();
  response.info.firmware_version = device_info->firmwareVersion();
  response.info.supported_min_sdk_version = device_info->supportedMinSdkVersion();
  response.success = true;
  return true;
}

bool OBCameraNode::getSDKVersionCallback(GetStringRequest& request, GetStringResponse& response) {
  (void)request;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  auto device_info = device_->getDeviceInfo();
  nlohmann::json data;
  data["firmware_version"] = device_info->firmwareVersion();
  data["supported_min_sdk_version"] = device_info->supportedMinSdkVersion();
  data["ros_sdk_version"] = OB_ROS_VERSION_STR;
  std::string major = std::to_string(ob::Version::getMajor());
  std::string minor = std::to_string(ob::Version::getMinor());
  std::string patch = std::to_string(ob::Version::getPatch());
  std::string version = major + "." + minor + "." + patch;
  data["ob_sdk_version"] = version;
  response.data = data.dump(2);
  response.success = true;
  return true;
}

bool OBCameraNode::toggleSensorCallback(std_srvs::SetBoolRequest& request,
                                        std_srvs::SetBoolResponse& response,
                                        const stream_index_pair& stream_index) {
  std::string msg;
  if (request.data) {
    if (enable_[stream_index]) {
      msg = stream_name_[stream_index] + " Already ON";
    }
    ROS_INFO_STREAM("toggling sensor " << stream_name_[stream_index] << " ON");
  } else {
    if (!enable_[stream_index]) {
      msg = stream_name_[stream_index] + " Already OFF";
    }
    ROS_INFO_STREAM("toggling sensor " << stream_name_[stream_index] << " OFF");
  }
  if (!msg.empty()) {
    ROS_INFO_STREAM(msg);
    response.success = false;
    response.message = msg;
    return false;
  }
  response.success = toggleSensor(stream_index, request.data, response.message);
  return response.success;
}

bool OBCameraNode::toggleSensor(const stream_index_pair& stream_index, bool enabled,
                                std::string& msg) {
  (void)msg;
  stopStreams();
  enable_[stream_index] = enabled;
  startStreams();
  return true;
}

bool OBCameraNode::saveImagesCallback(std_srvs::EmptyRequest& request,
                                      std_srvs::EmptyResponse& response) {
  (void)request;
  (void)response;
  for (const auto& stream_index : IMAGE_STREAMS) {
    if (enable_[stream_index]) {
      save_images_[stream_index] = true;
    } else {
      ROS_WARN_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    }
  }
  return true;
}

bool OBCameraNode::savePointCloudXYZCallback(std_srvs::EmptyRequest& request,
                                             std_srvs::EmptyResponse& response) {
  (void)request;
  (void)response;
  save_point_cloud_xyz_ = true;
  return true;
}

bool OBCameraNode::savePointCloudXYZRGBCallback(std_srvs::EmptyRequest& request,
                                                std_srvs::EmptyResponse& response) {
  (void)request;
  (void)response;
  save_point_cloud_xyzrgb_ = true;
  return true;
}

bool OBCameraNode::getCameraParamsCallback(orbbec_camera::GetCameraParamsRequest& request,
                                           orbbec_camera::GetCameraParamsResponse& response) {
  (void)request;
  try {
    auto camera_param = pipeline_->getCameraParam();
    response.l_intr_p[0] = camera_param.depthIntrinsic.fx;
    response.l_intr_p[1] = camera_param.depthIntrinsic.fy;
    response.l_intr_p[2] = camera_param.depthIntrinsic.cx;
    response.l_intr_p[3] = camera_param.depthIntrinsic.cy;
    response.r_intr_p[0] = camera_param.rgbIntrinsic.fx;
    response.r_intr_p[1] = camera_param.rgbIntrinsic.fy;
    response.r_intr_p[2] = camera_param.rgbIntrinsic.cx;
    response.r_intr_p[3] = camera_param.rgbIntrinsic.cy;
    for (int i = 0; i < 9; i++) {
      if (i < 3) {
        response.r2l_t[i] = camera_param.transform.trans[i];
      }
      response.r2l_r[i] = camera_param.transform.rot[i];
    }

  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to get camera params: " << e.getMessage());
    return false;
  }
  return true;
}

bool OBCameraNode::getSerialNumberCallback(GetStringRequest& request, GetStringResponse& response) {
  (void)request;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  try {
    auto device_info = device_->getDeviceInfo();
    response.data = device_info->serialNumber();
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to get serial number: " << e.getMessage());
    return false;
  }
  response.success = true;
  return true;
}

bool OBCameraNode::getDeviceTypeCallback(GetStringRequest& request, GetStringResponse& response) {
  (void)request;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  auto device_info = device_->getDeviceInfo();
  response.data = ObDeviceTypeToString(device_info->deviceType());
  response.success = true;
  return true;
}

bool OBCameraNode::getCameraInfoCallback(GetCameraInfoRequest& request,
                                         GetCameraInfoResponse& response,
                                         const stream_index_pair& stream_index) {
  (void)request;
  try {
    auto camera_param = pipeline_->getCameraParam();
    auto& intrinsic =
        stream_index == COLOR ? camera_param.rgbIntrinsic : camera_param.depthIntrinsic;
    auto& distortion =
        stream_index == COLOR ? camera_param.rgbDistortion : camera_param.depthDistortion;
    auto width = width_[stream_index];
    auto camera_info = convertToCameraInfo(intrinsic, distortion, width);
    response.info = camera_info;
  } catch (const ob::Error& e) {
    ROS_ERROR_STREAM("Failed to get camera info: " << e.getMessage());
    return false;
  }
  return true;
}

}  // namespace orbbec_camera
