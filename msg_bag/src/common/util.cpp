#include "util.h"

#include "can_msgs.pb.h"
#include "car_info_msgs.pb.h"
#include "esr_mobileye_msgs.pb.h"
#include "fisheye_msgs.pb.h"
#include "parking_msgs.pb.h"
#include "path_follower_msgs.pb.h"
#include "player.h"
#include "publish_subscribe.h"
#include "recorder.h"
#include "std_msgs.pb.h"
#include "twist_controller_msgs.pb.h"
#include "world_model_msgs.pb.h"

#include "timestamp.h"
#include <sys/timerfd.h>

namespace nullmax {
namespace msgbag {
std::map<std::string, HandlerFunc> recorder_handler_map;

void InitRecorderHandlerMap(Recorder *recorder) {
  recorder_handler_map.insert(
      {"topic_fisheye_pose",
       std::bind(&Recorder::MsgHandler<pb::fisheye_msgs::PoseMessage>, recorder,
                 "topic_fisheye_pose", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/parking/current_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/parking/current_pose", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/planning/pub_mission",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/planning/pub_mission", std::placeholders::_1)});
  // delphi_esr
  recorder_handler_map.insert(
      {"/delphi_esr/as_tx/radar_raw_tracks",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::RadarTrackArray>,
                 recorder, "/delphi_esr/as_tx/radar_raw_tracks",
                 std::placeholders::_1)});
  // mobileye
  recorder_handler_map.insert(
      {"/mobileye/parsed_tx/right_lka_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "/mobileye/parsed_tx/right_lka_lane",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/mobileye/parsed_tx/left_lka_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "/mobileye/parsed_tx/left_lka_lane",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/mobileye/parsed_tx/lane_detail",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LaneDetail>,
                 recorder, "/mobileye/parsed_tx/lane_detail",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/mobileye/parsed_tx/obstacle_array",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::ObstacleArray>,
                 recorder, "/mobileye/parsed_tx/obstacle_array",
                 std::placeholders::_1)});
  // vehicle report
  recorder_handler_map.insert(
      {"/vehicle/imu/data_raw",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::Imu>, recorder,
                 "/vehicle/imu/data_raw", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/gps/fix",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::NavSatFix>, recorder,
                 "/vehicle/gps/fix", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/gps/vel",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::TwistStamped>,
                 recorder, "/vehicle/gps/vel", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/gps/time",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::TimeReference>,
                 recorder, "/vehicle/gps/time", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/brake_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::BrakeReport>,
                 recorder, "/vehicle/brake_report", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/throttle_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::ThrottleReport>,
                 recorder, "/vehicle/throttle_report", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/steering_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SteeringReport>,
                 recorder, "/vehicle/steering_report", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/twist",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::TwistStamped>,
                 recorder, "/vehicle/twist", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/twist_raw",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::TwistStamped>,
                 recorder, "/vehicle/twist_raw", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/aceleration",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::Imu>, recorder,
                 "/vehicle/aceleration", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/gear_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::GearReport>, recorder,
                 "/vehicle/gear_report", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/misc_1_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::Misc1Report>,
                 recorder, "/vehicle/misc_1_report", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/wheel_speed_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::WheelSpeedReport>,
                 recorder, "/vehicle/wheel_speed_report",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/suspension_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SuspensionReport>,
                 recorder, "/vehicle/suspension_report",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/tire_pressure_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::TirePressureReport>,
                 recorder, "/vehicle/tire_pressure_report",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/fuel_level_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::FuelLevelReport>,
                 recorder, "/vehicle/fuel_level_report",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/surround_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SurroundReport>,
                 recorder, "/vehicle/surround_report", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/sonar_cloud",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::PointCloud2>,
                 recorder, "/vehicle/sonar_cloud", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/brake_info_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::BrakeInfoReport>,
                 recorder, "/vehicle/brake_info_report",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/throttle_info_report",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::ThrottleInfoReport>,
                 recorder, "/vehicle/throttle_info_report",
                 std::placeholders::_1)});
  // vehicle state
  recorder_handler_map.insert(
      {"/vehicle/vehicle_state",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::VehicleState>,
                 recorder, "/vehicle/vehicle_state", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/dbw_enabled",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/vehicle/dbw_enabled", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/joint_states",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::JointState>, recorder,
                 "/vehicle/joint_states", std::placeholders::_1)});
  // vehicle cmd
  recorder_handler_map.insert(
      {"/vehicle/enable",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/vehicle/enable", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/disable",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/vehicle/disable", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/brake_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::BrakeCmd>, recorder,
                 "/vehicle/brake_cmd", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/throttle_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::ThrottleCmd>,
                 recorder, "/vehicle/throttle_cmd", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/steering_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SteeringCmd>,
                 recorder, "/vehicle/steering_cmd", std::placeholders::_1)});

  // avp planner start
  recorder_handler_map.insert(
      {"/vehicle/gear_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::GearCmd>, recorder,
                 "/vehicle/gear_cmd", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/planning/pub_goal_reached",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/planning/pub_goal_reached", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/planning/AVP_planner_state",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::String>, recorder,
                 "/planning/AVP_planner_state", std::placeholders::_1)});
  // avp planner end
  recorder_handler_map.insert(
      {"/vehicle/turn_signal_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::TurnSignalCmd>,
                 recorder, "/vehicle/turn_signal_cmd", std::placeholders::_1)});

  // other
  recorder_handler_map.insert(
      {"/vehicle/fuzzy_pid",
       std::bind(&Recorder::MsgHandler<pb::twist_controller_msgs::FuzzyPid>,
                 recorder, "/vehicle/fuzzy_pid", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/planning/sim_gps/fix",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::NavSatFix>, recorder,
                 "/planning/sim_gps/fix", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/est_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/est_pose", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/gnss_stat", std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>,
                               recorder, "/gnss_stat", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/gnss_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/gnss_pose", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/osm/initpose_confirm",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/osm/initpose_confirm", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/osm/initpose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/osm/initpose", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/tf", std::bind(&Recorder::MsgHandler<pb::world_model_msgs::TFMessage>,
                        recorder, "/tf", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"ref_steering_angle",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Float64>, recorder,
                 "ref_steering_angle", std::placeholders::_1)});

  // path follower start
  recorder_handler_map.insert(
      {"follower_monitor",
       std::bind(
           &Recorder::MsgHandler<pb::path_follower_msgs::PathFollowerMonitor>,
           recorder, "follower_monitor", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"cmd_vel_stamped",
       std::bind(
           &Recorder::MsgHandler<pb::twist_controller_msgs::TwistCmdStamped>,
           recorder, "cmd_vel_stamped", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"twist_raw",
       std::bind(
           &Recorder::MsgHandler<pb::twist_controller_msgs::TwistCmdStamped>,
           recorder, "twist_raw", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"lqr_state",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::LQRState>,
                 recorder, "lqr_state", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"ref_waypoint",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::WaypointCell>,
                 recorder, "ref_waypoint", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"ref_waypoint_cent_lane",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::WaypointCell>,
                 recorder, "ref_waypoint_cent_lane", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"ref_steering_angle_cent_lane",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Float64>, recorder,
                 "ref_steering_angle_cent_lane", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"input_steering_angle",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Float64>, recorder,
                 "input_steering_angle", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"lqr_state_cent_lane",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::LQRState>,
                 recorder, "lqr_state_cent_lane", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"debug1", std::bind(&Recorder::MsgHandler<pb::std_msgs::Int8>, recorder,
                           "debug1", std::placeholders::_1)});
  // path follower end

  recorder_handler_map.insert(
      {"cent_subscribe_state",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Int8>, recorder,
                 "cent_subscribe_state", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"current_follower_method",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::PathFollower>,
                 recorder, "current_follower_method", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"follower_target",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::Point>, recorder,
                 "follower_target", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/perception/freespace_contour",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::FreeSpaceContour>,
                 recorder, "/perception/freespace_contour",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/fusion/obstacle_list",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::ObstacleArray>,
                 recorder, "/fusion/obstacle_list", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/predict/obstacle_predict",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::CutinPredictArray>,
                 recorder, "/predict/obstacle_predict",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/odom/current_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/odom/current_pose", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/global_planning/route_cmd",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::RouteCmd>,
                 recorder, "/global_planning/route_cmd",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/perception/left_lka_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "/perception/left_lka_lane",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/perception/right_lka_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "/perception/right_lka_lane",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"camera_info",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::SpeedLimit>,
                 recorder, "camera_info", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"navi_info",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::NaviInfo>,
                 recorder, "navi_info", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/cross_road",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::CrossRoad>,
                 recorder, "/cross_road", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"realtime_navi_info",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::NaviInfo>,
                 recorder, "realtime_navi_info", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"human_help_request",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::HumanHelp>,
                 recorder, "human_help_request", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"center_lane",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "center_lane", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"vehicle_monitor",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::VehicleMonitor>,
                 recorder, "vehicle_monitor", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"closest_waypoint",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::UInt32>, recorder,
                 "closest_waypoint", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/osm/route_cmd_confirm",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/osm/route_cmd_confirm", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"route_waypoints",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Lane>, recorder,
                 "route_waypoints", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/util/turn_signal_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::TurnSignalCmd>,
                 recorder, "/util/turn_signal_cmd", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/osm/replan_local_planner",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/osm/replan_local_planner", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"holistic_path",
       std::bind(&Recorder::MsgHandler<pb::esr_mobileye_msgs::LkaLane>,
                 recorder, "holistic_path", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"state", std::bind(&Recorder::MsgHandler<pb::std_msgs::String>, recorder,
                          "state", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"final_waypoints",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Trajectory>,
                 recorder, "final_waypoints", std::placeholders::_1)});
  // twist_controller
  recorder_handler_map.insert(
      {"control_monitor",
       std::bind(
           &Recorder::MsgHandler<pb::twist_controller_msgs::ControlMonitor>,
           recorder, "control_monitor", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/throttle_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::ThrottleCmd>,
                 recorder, "/vehicle/throttle_cmd", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/brake_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::BrakeCmd>, recorder,
                 "/vehicle/brake_cmd", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/vehicle/steering_cmd",
       std::bind(&Recorder::MsgHandler<pb::car_info_msgs::SteeringCmd>,
                 recorder, "/vehicle/steering_cmd", std::placeholders::_1)});
  // localization
  recorder_handler_map.insert(
      {"gnss_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "gnss_pose", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"gnss_stat", std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>,
                              recorder, "gnss_stat", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/osm/initpose_confirm",
       std::bind(&Recorder::MsgHandler<pb::std_msgs::Bool>, recorder,
                 "/osm/initpose_confirm", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/tf", std::bind(&Recorder::MsgHandler<pb::world_model_msgs::TFMessage>,
                        recorder, "/tf", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/est_pose",
       std::bind(&Recorder::MsgHandler<pb::geometry_msgs::PoseStamped>,
                 recorder, "/est_pose", std::placeholders::_1)});
  // global_planning
  recorder_handler_map.insert(
      {"/global_viapoints_visualization",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Marker>, recorder,
                 "/global_viapoints_visualization", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/planning/global_viapoints",
       std::bind(&Recorder::MsgHandler<pb::parking_msgs::GlobalViapoints>,
                 recorder, "/planning/global_viapoints",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/planning/pub_global_viapoints",
       std::bind(&Recorder::MsgHandler<pb::parking_msgs::GlobalViapoints>,
                 recorder, "/planning/pub_global_viapoints",
                 std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/car_pose",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Marker>, recorder,
                 "/car_pose", std::placeholders::_1)});
  recorder_handler_map.insert(
      {"/goal_pose",
       std::bind(&Recorder::MsgHandler<pb::world_model_msgs::Marker>, recorder,
                 "/goal_pose", std::placeholders::_1)});
};
/////////////////////////////////player_handler_map
std::map<std::string, HandlerFunc> player_handler_map;

void InitPlayerHandlerMap(Player *Player) {
  player_handler_map.insert(
      {"topic_fisheye_pose",
       std::bind(&Player::MsgHandler<pb::fisheye_msgs::PoseMessage>, Player,
                 "topic_fisheye_pose", std::placeholders::_1)});
  player_handler_map.insert(
      {"/parking/current_pose",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::PoseStamped>, Player,
                 "/parking/current_pose", std::placeholders::_1)});
  player_handler_map.insert(
      {"/planning/pub_mission",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::PoseStamped>, Player,
                 "/planning/pub_mission", std::placeholders::_1)});
  // delphi_esr
  player_handler_map.insert(
      {"/delphi_esr/as_tx/radar_raw_tracks",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::RadarTrackArray>,
                 Player, "/delphi_esr/as_tx/radar_raw_tracks",
                 std::placeholders::_1)});
  // mobileye
  player_handler_map.insert(
      {"/mobileye/parsed_tx/right_lka_lane",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::LkaLane>, Player,
                 "/mobileye/parsed_tx/right_lka_lane", std::placeholders::_1)});
  player_handler_map.insert(
      {"/mobileye/parsed_tx/left_lka_lane",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::LkaLane>, Player,
                 "/mobileye/parsed_tx/left_lka_lane", std::placeholders::_1)});
  player_handler_map.insert(
      {"/mobileye/parsed_tx/lane_detail",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::LaneDetail>, Player,
                 "/mobileye/parsed_tx/lane_detail", std::placeholders::_1)});
  player_handler_map.insert(
      {"/mobileye/parsed_tx/obstacle_array",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::ObstacleArray>,
                 Player, "/mobileye/parsed_tx/obstacle_array",
                 std::placeholders::_1)});
  // vehicle report
  player_handler_map.insert(
      {"/vehicle/imu/data_raw",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::Imu>, Player,
                 "/vehicle/imu/data_raw", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/gps/fix",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::NavSatFix>, Player,
                 "/vehicle/gps/fix", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/gps/vel",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::TwistStamped>, Player,
                 "/vehicle/gps/vel", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/gps/time",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::TimeReference>, Player,
                 "/vehicle/gps/time", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/brake_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::BrakeReport>, Player,
                 "/vehicle/brake_report", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/throttle_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::ThrottleReport>, Player,
                 "/vehicle/throttle_report", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/steering_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::SteeringReport>, Player,
                 "/vehicle/steering_report", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/twist",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::TwistStamped>, Player,
                 "/vehicle/twist", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/twist_raw",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::TwistStamped>, Player,
                 "/vehicle/twist_raw", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/aceleration",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::Imu>, Player,
                 "/vehicle/aceleration", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/gear_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::GearReport>, Player,
                 "/vehicle/gear_report", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/misc_1_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::Misc1Report>, Player,
                 "/vehicle/misc_1_report", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/wheel_speed_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::WheelSpeedReport>,
                 Player, "/vehicle/wheel_speed_report",
                 std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/suspension_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::SuspensionReport>,
                 Player, "/vehicle/suspension_report", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/tire_pressure_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::TirePressureReport>,
                 Player, "/vehicle/tire_pressure_report",
                 std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/fuel_level_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::FuelLevelReport>,
                 Player, "/vehicle/fuel_level_report", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/surround_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::SurroundReport>, Player,
                 "/vehicle/surround_report", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/sonar_cloud",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::PointCloud2>, Player,
                 "/vehicle/sonar_cloud", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/brake_info_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::BrakeInfoReport>,
                 Player, "/vehicle/brake_info_report", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/throttle_info_report",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::ThrottleInfoReport>,
                 Player, "/vehicle/throttle_info_report",
                 std::placeholders::_1)});
  // vehicle state
  player_handler_map.insert(
      {"/vehicle/vehicle_state",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::VehicleState>, Player,
                 "/vehicle/vehicle_state", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/dbw_enabled",
       std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                 "/vehicle/dbw_enabled", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/joint_states",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::JointState>, Player,
                 "/vehicle/joint_states", std::placeholders::_1)});
  // vehicle cmd
  player_handler_map.insert(
      {"/vehicle/enable",
       std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                 "/vehicle/enable", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/disable",
       std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                 "/vehicle/disable", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/brake_cmd",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::BrakeCmd>, Player,
                 "/vehicle/brake_cmd", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/throttle_cmd",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::ThrottleCmd>, Player,
                 "/vehicle/throttle_cmd", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/steering_cmd",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::SteeringCmd>, Player,
                 "/vehicle/steering_cmd", std::placeholders::_1)});

  // avp planner start
  player_handler_map.insert(
      {"/vehicle/gear_cmd",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::GearCmd>, Player,
                 "/vehicle/gear_cmd", std::placeholders::_1)});
  player_handler_map.insert(
      {"/planning/pub_goal_reached",
       std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                 "/planning/pub_goal_reached", std::placeholders::_1)});
  player_handler_map.insert(
      {"/planning/AVP_planner_state",
       std::bind(&Player::MsgHandler<pb::std_msgs::String>, Player,
                 "/planning/AVP_planner_state", std::placeholders::_1)});
  // avp planner end
  player_handler_map.insert(
      {"/vehicle/turn_signal_cmd",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::TurnSignalCmd>, Player,
                 "/vehicle/turn_signal_cmd", std::placeholders::_1)});

  // other
  player_handler_map.insert(
      {"/vehicle/fuzzy_pid",
       std::bind(&Player::MsgHandler<pb::twist_controller_msgs::FuzzyPid>,
                 Player, "/vehicle/fuzzy_pid", std::placeholders::_1)});
  player_handler_map.insert(
      {"/planning/sim_gps/fix",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::NavSatFix>, Player,
                 "/planning/sim_gps/fix", std::placeholders::_1)});
  player_handler_map.insert(
      {"/est_pose",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::PoseStamped>, Player,
                 "/est_pose", std::placeholders::_1)});
  player_handler_map.insert(
      {"/gnss_stat", std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                               "/gnss_stat", std::placeholders::_1)});
  player_handler_map.insert(
      {"/gnss_pose",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::PoseStamped>, Player,
                 "/gnss_pose", std::placeholders::_1)});
  player_handler_map.insert(
      {"/osm/initpose_confirm",
       std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                 "/osm/initpose_confirm", std::placeholders::_1)});
  player_handler_map.insert(
      {"/osm/initpose",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::PoseStamped>, Player,
                 "/osm/initpose", std::placeholders::_1)});
  player_handler_map.insert(
      {"/tf", std::bind(&Player::MsgHandler<pb::world_model_msgs::TFMessage>,
                        Player, "/tf", std::placeholders::_1)});
  player_handler_map.insert(
      {"ref_steering_angle",
       std::bind(&Player::MsgHandler<pb::std_msgs::Float64>, Player,
                 "ref_steering_angle", std::placeholders::_1)});

  // path follower start
  player_handler_map.insert(
      {"follower_monitor",
       std::bind(
           &Player::MsgHandler<pb::path_follower_msgs::PathFollowerMonitor>,
           Player, "follower_monitor", std::placeholders::_1)});
  player_handler_map.insert(
      {"cmd_vel_stamped",
       std::bind(
           &Player::MsgHandler<pb::twist_controller_msgs::TwistCmdStamped>,
           Player, "cmd_vel_stamped", std::placeholders::_1)});
  player_handler_map.insert(
      {"twist_raw",
       std::bind(
           &Player::MsgHandler<pb::twist_controller_msgs::TwistCmdStamped>,
           Player, "twist_raw", std::placeholders::_1)});
  player_handler_map.insert(
      {"lqr_state",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::LQRState>, Player,
                 "lqr_state", std::placeholders::_1)});
  player_handler_map.insert(
      {"ref_waypoint",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::WaypointCell>,
                 Player, "ref_waypoint", std::placeholders::_1)});
  player_handler_map.insert(
      {"ref_waypoint_cent_lane",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::WaypointCell>,
                 Player, "ref_waypoint_cent_lane", std::placeholders::_1)});
  player_handler_map.insert(
      {"ref_steering_angle_cent_lane",
       std::bind(&Player::MsgHandler<pb::std_msgs::Float64>, Player,
                 "ref_steering_angle_cent_lane", std::placeholders::_1)});
  player_handler_map.insert(
      {"input_steering_angle",
       std::bind(&Player::MsgHandler<pb::std_msgs::Float64>, Player,
                 "input_steering_angle", std::placeholders::_1)});
  player_handler_map.insert(
      {"lqr_state_cent_lane",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::LQRState>, Player,
                 "lqr_state_cent_lane", std::placeholders::_1)});
  player_handler_map.insert(
      {"debug1", std::bind(&Player::MsgHandler<pb::std_msgs::Int8>, Player,
                           "debug1", std::placeholders::_1)});
  // path follower end

  player_handler_map.insert(
      {"cent_subscribe_state",
       std::bind(&Player::MsgHandler<pb::std_msgs::Int8>, Player,
                 "cent_subscribe_state", std::placeholders::_1)});
  player_handler_map.insert(
      {"current_follower_method",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::PathFollower>,
                 Player, "current_follower_method", std::placeholders::_1)});
  player_handler_map.insert(
      {"follower_target",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::Point>, Player,
                 "follower_target", std::placeholders::_1)});
  player_handler_map.insert(
      {"/perception/freespace_contour",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::FreeSpaceContour>,
                 Player, "/perception/freespace_contour",
                 std::placeholders::_1)});
  player_handler_map.insert(
      {"/fusion/obstacle_list",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::ObstacleArray>,
                 Player, "/fusion/obstacle_list", std::placeholders::_1)});
  player_handler_map.insert(
      {"/predict/obstacle_predict",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::CutinPredictArray>,
                 Player, "/predict/obstacle_predict", std::placeholders::_1)});
  player_handler_map.insert(
      {"/odom/current_pose",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::PoseStamped>, Player,
                 "/odom/current_pose", std::placeholders::_1)});
  player_handler_map.insert(
      {"/global_planning/route_cmd",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::RouteCmd>, Player,
                 "/global_planning/route_cmd", std::placeholders::_1)});
  player_handler_map.insert(
      {"/perception/left_lka_lane",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::LkaLane>, Player,
                 "/perception/left_lka_lane", std::placeholders::_1)});
  player_handler_map.insert(
      {"/perception/right_lka_lane",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::LkaLane>, Player,
                 "/perception/right_lka_lane", std::placeholders::_1)});
  player_handler_map.insert(
      {"camera_info",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::SpeedLimit>, Player,
                 "camera_info", std::placeholders::_1)});
  player_handler_map.insert(
      {"navi_info",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::NaviInfo>, Player,
                 "navi_info", std::placeholders::_1)});
  player_handler_map.insert(
      {"/cross_road",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::CrossRoad>, Player,
                 "/cross_road", std::placeholders::_1)});
  player_handler_map.insert(
      {"realtime_navi_info",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::NaviInfo>, Player,
                 "realtime_navi_info", std::placeholders::_1)});
  player_handler_map.insert(
      {"human_help_request",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::HumanHelp>, Player,
                 "human_help_request", std::placeholders::_1)});
  player_handler_map.insert(
      {"center_lane",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::LkaLane>, Player,
                 "center_lane", std::placeholders::_1)});
  player_handler_map.insert(
      {"vehicle_monitor",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::VehicleMonitor>,
                 Player, "vehicle_monitor", std::placeholders::_1)});
  player_handler_map.insert(
      {"closest_waypoint",
       std::bind(&Player::MsgHandler<pb::std_msgs::UInt32>, Player,
                 "closest_waypoint", std::placeholders::_1)});
  player_handler_map.insert(
      {"/osm/route_cmd_confirm",
       std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                 "/osm/route_cmd_confirm", std::placeholders::_1)});
  player_handler_map.insert(
      {"route_waypoints",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::Lane>, Player,
                 "route_waypoints", std::placeholders::_1)});
  player_handler_map.insert(
      {"/util/turn_signal_cmd",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::TurnSignalCmd>, Player,
                 "/util/turn_signal_cmd", std::placeholders::_1)});
  player_handler_map.insert(
      {"/osm/replan_local_planner",
       std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                 "/osm/replan_local_planner", std::placeholders::_1)});
  player_handler_map.insert(
      {"holistic_path",
       std::bind(&Player::MsgHandler<pb::esr_mobileye_msgs::LkaLane>, Player,
                 "holistic_path", std::placeholders::_1)});
  player_handler_map.insert(
      {"state", std::bind(&Player::MsgHandler<pb::std_msgs::String>, Player,
                          "state", std::placeholders::_1)});
  player_handler_map.insert(
      {"final_waypoints",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::Trajectory>, Player,
                 "final_waypoints", std::placeholders::_1)});
  // twist_controller
  player_handler_map.insert(
      {"control_monitor",
       std::bind(&Player::MsgHandler<pb::twist_controller_msgs::ControlMonitor>,
                 Player, "control_monitor", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/throttle_cmd",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::ThrottleCmd>, Player,
                 "/vehicle/throttle_cmd", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/brake_cmd",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::BrakeCmd>, Player,
                 "/vehicle/brake_cmd", std::placeholders::_1)});
  player_handler_map.insert(
      {"/vehicle/steering_cmd",
       std::bind(&Player::MsgHandler<pb::car_info_msgs::SteeringCmd>, Player,
                 "/vehicle/steering_cmd", std::placeholders::_1)});
  // localization
  player_handler_map.insert(
      {"gnss_pose",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::PoseStamped>, Player,
                 "gnss_pose", std::placeholders::_1)});
  player_handler_map.insert(
      {"gnss_stat", std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                              "gnss_stat", std::placeholders::_1)});
  player_handler_map.insert(
      {"/osm/initpose_confirm",
       std::bind(&Player::MsgHandler<pb::std_msgs::Bool>, Player,
                 "/osm/initpose_confirm", std::placeholders::_1)});
  player_handler_map.insert(
      {"/tf", std::bind(&Player::MsgHandler<pb::world_model_msgs::TFMessage>,
                        Player, "/tf", std::placeholders::_1)});
  player_handler_map.insert(
      {"/est_pose",
       std::bind(&Player::MsgHandler<pb::geometry_msgs::PoseStamped>, Player,
                 "/est_pose", std::placeholders::_1)});
  // global_planning
  player_handler_map.insert(
      {"/global_viapoints_visualization",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::Marker>, Player,
                 "/global_viapoints_visualization", std::placeholders::_1)});
  player_handler_map.insert(
      {"/planning/global_viapoints",
       std::bind(&Player::MsgHandler<pb::parking_msgs::GlobalViapoints>, Player,
                 "/planning/global_viapoints", std::placeholders::_1)});
  player_handler_map.insert(
      {"/planning/pub_global_viapoints",
       std::bind(&Player::MsgHandler<pb::parking_msgs::GlobalViapoints>, Player,
                 "/planning/pub_global_viapoints", std::placeholders::_1)});
  player_handler_map.insert(
      {"/car_pose", std::bind(&Player::MsgHandler<pb::world_model_msgs::Marker>,
                              Player, "/car_pose", std::placeholders::_1)});
  player_handler_map.insert(
      {"/goal_pose",
       std::bind(&Player::MsgHandler<pb::world_model_msgs::Marker>, Player,
                 "/goal_pose", std::placeholders::_1)});
};

uint32_t WriteHeader(const HeaderType &header, Buffer &buffer) {
  uint32_t header_len = 0;
  LOG_DEBUG("write header,header size:{}", header.size());
  for (auto field : header) {
    std::string field_str;
    field_str.append(field.first).append("=").append(field.second);
    LOG_DEBUG("header field:{}", field_str);
    uint32_t field_str_size = field_str.size();
    buffer.appendInt32(field_str_size);
    buffer.append(field_str);
    header_len += sizeof(uint32_t) + field_str_size;
  }
  LOG_DEBUG("write header,buffer readablebytes:{}", buffer.readableBytes());
  buffer.prependInt32(header_len);
  LOG_DEBUG("write header2,buffer readablebytes:{}", buffer.readableBytes());
  return header_len;
}

uint32_t ReadHeader(HeaderType &header, Buffer &buffer) {
  uint32_t header_len = buffer.readInt32();
  std::cout << "header len:" << header_len << std::endl;
  uint32_t read_len = 0;
  while (read_len < header_len) {
    uint32_t field_len = buffer.readInt32();
    std::cout << "field_len:" << field_len << std::endl;
    std::string field_str = buffer.retrieveAsString(field_len);
    std::size_t pos = field_str.find_first_of("=");
    if (pos == std::string::npos) {
      std::cout << field_str << " can't find =" << std::endl;
      continue;
    }
    header[field_str.substr(0, pos)] =
        field_str.substr(pos + 1, field_str.size() - pos);
    read_len += field_len + sizeof(uint32_t);
  }
  std::cout << "header size:" << header.size() << std::endl;
  std::cout << "read len:" << read_len << std::endl;

  return header_len;
}

std::string HexToString(const unsigned char hex) {
  char hex_str[5] = {0};
  sprintf(hex_str, "0x%02x", hex);
  return hex_str;
}

int CreateTimerfd() {
  int timerfd =
      ::timerfd_create(CLOCK_REALTIME, /*TFD_NONBLOCK |*/ TFD_CLOEXEC);
  if (timerfd < 0) {
    LOG_ERROR("Failed in timerfd_create");
  }
  return timerfd;
}

struct timespec HowMuchTimeFromNow(Timestamp when) {
  int64_t nanoseconds =
      when.NanoSecondsSinceEpoch() - Timestamp::Now().NanoSecondsSinceEpoch();
  if (nanoseconds < 100) {
    nanoseconds = 100;
  }
  struct timespec ts;
  ts.tv_sec =
      static_cast<time_t>(nanoseconds / Timestamp::NANO_SECONDS_PER_SECOND);
  ts.tv_nsec =
      static_cast<long>(nanoseconds % Timestamp::NANO_SECONDS_PER_SECOND);
  return ts;
}

void ReadTimerfd(int timerfd, Timestamp now) {
  uint64_t howmany;
  ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
  LOG_DEBUG("TimerQueue::handleRead() {} at {}", howmany, now.ToString());
  if (n != sizeof howmany) {
    LOG_ERROR("TimerQueue::handleRead() reads {} bytes instead of 8", n);
  }
}

void ResetTimerfd(int timerfd, Timestamp expiration) {
  // wake up loop by timerfd_settime()
  struct itimerspec new_value;
  struct itimerspec old_value;
  bzero(&new_value, sizeof new_value);
  bzero(&old_value, sizeof old_value);
  new_value.it_value = HowMuchTimeFromNow(expiration);
  int ret = ::timerfd_settime(timerfd, 0, &new_value, &old_value);
  if (ret) {
    LOG_ERROR("timerfd_settime()");
  }
}

} // namespace msgbag
} // namespace nullmax