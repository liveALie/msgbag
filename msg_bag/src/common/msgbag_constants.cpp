#include "msgbag_constants.h"

#include "can_msgs.pb.h"
#include "car_info_msgs.pb.h"
#include "esr_mobileye_msgs.pb.h"
#include "fisheye_msgs.pb.h"
#include "parking_msgs.pb.h"
#include "path_follower_msgs.pb.h"
#include "publish_subscribe.h"
#include "recorder.h"
#include "std_msgs.pb.h"
#include "twist_controller_msgs.pb.h"
#include "world_model_msgs.pb.h"

namespace nullmax {
namespace msgbag {

std::map<std::string, HandlerFunc> msg_handler_map;

void InitHandlerMap(Recorder *recorder) {
  msg_handler_map.insert(
      {"topic_fisheye_pose",
       std::bind(&Recorder::MsgHandler<pb::fisheye_msgs::PoseMessage>, recorder,
                 "topic_fisheye_pose", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/parking/current_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/parking/current_pose", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/planning/pub_mission",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/planning/pub_mission", std::placeholders::_1)});
  // delphi_esr
  msg_handler_map.insert(
      {"/delphi_esr/as_tx/radar_raw_tracks",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::RadarTrackArray>,
                 recorder, "/delphi_esr/as_tx/radar_raw_tracks",
                 std::placeholders::_1)});
  // mobileye
  msg_handler_map.insert(
      {"/mobileye/parsed_tx/right_lka_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "/mobileye/parsed_tx/right_lka_lane",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/mobileye/parsed_tx/left_lka_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "/mobileye/parsed_tx/left_lka_lane",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/mobileye/parsed_tx/lane_detail",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LaneDetail>,
                 recorder, "/mobileye/parsed_tx/lane_detail",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/mobileye/parsed_tx/obstacle_array",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::ObstacleArray>,
                 recorder, "/mobileye/parsed_tx/obstacle_array",
                 std::placeholders::_1)});
  // vehicle report
  msg_handler_map.insert(
      {"/vehicle/imu/data_raw",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::Imu>, recorder,
                 "/vehicle/imu/data_raw", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/gps/fix",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::NavSatFix>, recorder,
                 "/vehicle/gps/fix", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/gps/vel",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::TwistStamped>,
                 recorder, "/vehicle/gps/vel", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/gps/time",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::TimeReference>,
                 recorder, "/vehicle/gps/time", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/brake_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::BrakeReport>,
                 recorder, "/vehicle/brake_report", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/throttle_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::ThrottleReport>,
                 recorder, "/vehicle/throttle_report", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/steering_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SteeringReport>,
                 recorder, "/vehicle/steering_report", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/twist",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::TwistStamped>,
                 recorder, "/vehicle/twist", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/twist_raw",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::TwistStamped>,
                 recorder, "/vehicle/twist_raw", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/aceleration",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::Imu>, recorder,
                 "/vehicle/aceleration", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/gear_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::GearReport>, recorder,
                 "/vehicle/gear_report", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/misc_1_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::Misc1Report>,
                 recorder, "/vehicle/misc_1_report", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/wheel_speed_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::WheelSpeedReport>,
                 recorder, "/vehicle/wheel_speed_report",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/suspension_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SuspensionReport>,
                 recorder, "/vehicle/suspension_report",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/tire_pressure_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::TirePressureReport>,
                 recorder, "/vehicle/tire_pressure_report",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/fuel_level_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::FuelLevelReport>,
                 recorder, "/vehicle/fuel_level_report",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/surround_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SurroundReport>,
                 recorder, "/vehicle/surround_report", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/sonar_cloud",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::PointCloud2>,
                 recorder, "/vehicle/sonar_cloud", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/brake_info_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::BrakeInfoReport>,
                 recorder, "/vehicle/brake_info_report",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/throttle_info_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::ThrottleInfoReport>,
                 recorder, "/vehicle/throttle_info_report",
                 std::placeholders::_1)});
  // vehicle state
  msg_handler_map.insert(
      {"/vehicle/vehicle_state",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::VehicleState>,
                 recorder, "/vehicle/vehicle_state", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/dbw_enabled",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/vehicle/dbw_enabled", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/joint_states",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::JointState>, recorder,
                 "/vehicle/joint_states", std::placeholders::_1)});
  // vehicle cmd
  msg_handler_map.insert(
      {"/vehicle/enable",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/vehicle/enable", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/disable",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/vehicle/disable", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/brake_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::BrakeCmd>, recorder,
                 "/vehicle/brake_cmd", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/throttle_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::ThrottleCmd>,
                 recorder, "/vehicle/throttle_cmd", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/steering_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SteeringCmd>,
                 recorder, "/vehicle/steering_cmd", std::placeholders::_1)});

  // avp planner start
  msg_handler_map.insert(
      {"/vehicle/gear_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::GearCmd>, recorder,
                 "/vehicle/gear_cmd", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/planning/pub_goal_reached",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/planning/pub_goal_reached", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/planning/AVP_planner_state",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::String>, recorder,
                 "/planning/AVP_planner_state", std::placeholders::_1)});
  // avp planner end
  msg_handler_map.insert(
      {"/vehicle/turn_signal_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::TurnSignalCmd>,
                 recorder, "/vehicle/turn_signal_cmd", std::placeholders::_1)});

  // other
  msg_handler_map.insert(
      {"/vehicle/fuzzy_pid",
       std::bind(&Recorder::MsgHandler<pb::twist_controller_msgs::FuzzyPid>,
                 recorder, "/vehicle/fuzzy_pid", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/planning/sim_gps/fix",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::NavSatFix>, recorder,
                 "/planning/sim_gps/fix", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/est_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/est_pose", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/gnss_stat", std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>,
                               recorder, "/gnss_stat", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/gnss_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/gnss_pose", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/osm/initpose_confirm",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/osm/initpose_confirm", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/osm/initpose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/osm/initpose", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/tf", std::bind(&Recorder::MsgHandler<pb::world_model_msgs::TFMessage>,
                        recorder, "/tf", std::placeholders::_1)});
  msg_handler_map.insert(
      {"ref_steering_angle",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Float64>, recorder,
                 "ref_steering_angle", std::placeholders::_1)});

  // path follower start
  msg_handler_map.insert(
      {"follower_monitor",
       std::bind(
           &Recorder::MsgHandler<pb::path_follower_msgs::PathFollowerMonitor>,
           recorder, "follower_monitor", std::placeholders::_1)});
  msg_handler_map.insert(
      {"cmd_vel_stamped",
       std::bind(
           &Recorder::MsgHandler<pb::twist_controller_msgs::TwistCmdStamped>,
           recorder, "cmd_vel_stamped", std::placeholders::_1)});
  msg_handler_map.insert(
      {"twist_raw",
       std::bind(
           &Recorder::MsgHandler<pb::twist_controller_msgs::TwistCmdStamped>,
           recorder, "twist_raw", std::placeholders::_1)});
  msg_handler_map.insert(
      {"lqr_state",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::LQRState>,
                 recorder, "lqr_state", std::placeholders::_1)});
  msg_handler_map.insert(
      {"ref_waypoint",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::WaypointCell>,
                 recorder, "ref_waypoint", std::placeholders::_1)});
  msg_handler_map.insert(
      {"ref_waypoint_cent_lane",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::WaypointCell>,
                 recorder, "ref_waypoint_cent_lane", std::placeholders::_1)});
  msg_handler_map.insert(
      {"ref_steering_angle_cent_lane",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Float64>, recorder,
                 "ref_steering_angle_cent_lane", std::placeholders::_1)});
  msg_handler_map.insert(
      {"input_steering_angle",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Float64>, recorder,
                 "input_steering_angle", std::placeholders::_1)});
  msg_handler_map.insert(
      {"lqr_state_cent_lane",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::LQRState>,
                 recorder, "lqr_state_cent_lane", std::placeholders::_1)});
  msg_handler_map.insert(
      {"debug1", std::bind(&Recorder::MsgHandler<pb::std_msgs::Int8>, recorder,
                           "debug1", std::placeholders::_1)});
  // path follower end

  msg_handler_map.insert(
      {"cent_subscribe_state",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Int8>, recorder,
                 "cent_subscribe_state", std::placeholders::_1)});
  msg_handler_map.insert(
      {"current_follower_method",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::PathFollower>,
                 recorder, "current_follower_method", std::placeholders::_1)});
  msg_handler_map.insert(
      {"follower_target",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::Point>, recorder,
                 "follower_target", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/perception/freespace_contour",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::FreeSpaceContour>,
                 recorder, "/perception/freespace_contour",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/fusion/obstacle_list",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::ObstacleArray>,
                 recorder, "/fusion/obstacle_list", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/predict/obstacle_predict",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::CutinPredictArray>,
                 recorder, "/predict/obstacle_predict",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/odom/current_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/odom/current_pose", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/global_planning/route_cmd",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::RouteCmd>,
                 recorder, "/global_planning/route_cmd",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/perception/left_lka_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "/perception/left_lka_lane",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/perception/right_lka_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "/perception/right_lka_lane",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"camera_info",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::SpeedLimit>,
                 recorder, "camera_info", std::placeholders::_1)});
  msg_handler_map.insert(
      {"navi_info",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::NaviInfo>,
                 recorder, "navi_info", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/cross_road",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::CrossRoad>,
                 recorder, "/cross_road", std::placeholders::_1)});
  msg_handler_map.insert(
      {"realtime_navi_info",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::NaviInfo>,
                 recorder, "realtime_navi_info", std::placeholders::_1)});
  msg_handler_map.insert(
      {"human_help_request",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::HumanHelp>,
                 recorder, "human_help_request", std::placeholders::_1)});
  msg_handler_map.insert(
      {"center_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "center_lane", std::placeholders::_1)});
  msg_handler_map.insert(
      {"vehicle_monitor",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::VehicleMonitor>,
                 recorder, "vehicle_monitor", std::placeholders::_1)});
  msg_handler_map.insert(
      {"closest_waypoint",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::UInt32>, recorder,
                 "closest_waypoint", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/osm/route_cmd_confirm",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/osm/route_cmd_confirm", std::placeholders::_1)});
  msg_handler_map.insert(
      {"route_waypoints",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Lane>, recorder,
                 "route_waypoints", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/util/turn_signal_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::TurnSignalCmd>,
                 recorder, "/util/turn_signal_cmd", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/osm/replan_local_planner",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/osm/replan_local_planner", std::placeholders::_1)});
  msg_handler_map.insert(
      {"holistic_path",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "holistic_path", std::placeholders::_1)});
  msg_handler_map.insert(
      {"state", std::bind(&Recorder::MsgHandler<pb::std_msgs::String>, recorder,
                          "state", std::placeholders::_1)});
  msg_handler_map.insert(
      {"final_waypoints",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Trajectory>,
                 recorder, "final_waypoints", std::placeholders::_1)});
  // twist_controller
  msg_handler_map.insert(
      {"control_monitor",
       std::bind(
           &Recorder::MsgHandler<pb::twist_controller_msgs::ControlMonitor>,
           recorder, "control_monitor", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/throttle_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::ThrottleCmd>,
                 recorder, "/vehicle/throttle_cmd", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/brake_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::BrakeCmd>, recorder,
                 "/vehicle/brake_cmd", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/vehicle/steering_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SteeringCmd>,
                 recorder, "/vehicle/steering_cmd", std::placeholders::_1)});
  // localization
  msg_handler_map.insert(
      {"gnss_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "gnss_pose", std::placeholders::_1)});
  msg_handler_map.insert(
      {"gnss_stat", std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>,
                              recorder, "gnss_stat", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/osm/initpose_confirm",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/osm/initpose_confirm", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/tf", std::bind(&Recorder::MsgHandler<pb::world_model_msgs::TFMessage>,
                        recorder, "/tf", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/est_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/est_pose", std::placeholders::_1)});
  // global_planning
  msg_handler_map.insert(
      {"/global_viapoints_visualization",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Marker>, recorder,
                 "/global_viapoints_visualization", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/planning/global_viapoints",
       std::bind(&Recorder::MsgHandler<pb::parking_msgs::GlobalViapoints>,
                 recorder, "/planning/global_viapoints",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/planning/pub_global_viapoints",
       std::bind(&Recorder::MsgHandler<pb::parking_msgs::GlobalViapoints>,
                 recorder, "/planning/pub_global_viapoints",
                 std::placeholders::_1)});
  msg_handler_map.insert(
      {"/car_pose",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Marker>, recorder,
                 "/car_pose", std::placeholders::_1)});
  msg_handler_map.insert(
      {"/goal_pose",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Marker>, recorder,
                 "/goal_pose", std::placeholders::_1)});
};
} // namespace msgbag
} // namespace nullmax