#include <iostream>
#include <cmath>
#include <algorithm>
#include<string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


typedef struct{
    double total_takeoff_weight_g;
    double wing_loading;
   
    double main_aspect_ratio;
    double total_moment_arm;
    double horiz_tail_vol_coeff;
    double horiz_aspect_ratio;
    double vert_tail_vol_coeff;
    double vert_aspect_ratio;
    double max_lift_coeff;
    double angle_of_attack;
    double wing_tunnel_velocity;
    double payload_balance_shift;
    
    
    
    
} UAVConstraints;

const double rho = 1.225;
const double g = 9.81;
const double Nu = 1.5e-5;
const double CDO = 0.02;


 const double  taper_ratio = 0.5;

typedef struct {
    double total_weight_kg;
    double wing_area;
    double wing_span;
    double MAC;
    double v_stall;
    double v_cruise;
    double cl;
    double cd;
    double drag_force;
    double horiz_wing_area;
         double horiz_wing_span;
         double horiz_mac;
         double horiz_chord_tip;
         double horiz_chord_root;
      double elevator_breadth;
      double elevator_length;   
      double vert_wing_area;
      double vert_wing_span;
      double vert_mac;
      double vert_chord_root;
      double vert_chord_tip;
      double vert_height;
      double reynolds_num;
      double lift;


    
} UAVoutputs;

void calculate_UAV_metrics(const UAVConstraints* inputs, UAVoutputs* outputs);
void display_results( const UAVoutputs* outputs);

  int main(int argc, char* argv[]) 
{
   
    UAVConstraints initial_specs;
    if (argc >= 13) {
        initial_specs.total_takeoff_weight_g = std::stod(argv[1]);
        initial_specs.wing_loading           = std::stod(argv[2]);
        initial_specs.main_aspect_ratio      = std::stod(argv[3]);
        initial_specs.total_moment_arm       = std::stod(argv[4]);
        initial_specs.horiz_tail_vol_coeff   = std::stod(argv[5]);
        initial_specs.horiz_aspect_ratio     = std::stod(argv[6]);
        initial_specs.vert_tail_vol_coeff    = std::stod(argv[7]);
        initial_specs.vert_aspect_ratio      = std::stod(argv[8]);
        initial_specs.max_lift_coeff         = std::stod(argv[9]);
        initial_specs.angle_of_attack        = std::stod(argv[10]);
        initial_specs.wing_tunnel_velocity   = std::stod(argv[11]);
        initial_specs.payload_balance_shift  = std::stod(argv[12]);
    } else {
        initial_specs.total_takeoff_weight_g = 700.0;
        initial_specs.wing_loading           = 37.50;
        initial_specs.main_aspect_ratio      = 9.0;
        initial_specs.total_moment_arm       = 1.0;
        initial_specs.horiz_tail_vol_coeff   = 0.5;
        initial_specs.horiz_aspect_ratio     = 4.0;
        initial_specs.vert_tail_vol_coeff    = 0.04;
        initial_specs.vert_aspect_ratio      = 1.6;
        initial_specs.max_lift_coeff         = 1.50;
        initial_specs.angle_of_attack        = 5.0;
        initial_specs.wing_tunnel_velocity   = 15.0;
        initial_specs.payload_balance_shift  = 0.0;
    }

    UAVoutputs Calculated_results;
    calculate_UAV_metrics(&initial_specs, &Calculated_results);
  display_results(&Calculated_results);
    
   

    return 0;
}

void calculate_UAV_metrics(const UAVConstraints* inputs, UAVoutputs* outputs){
    outputs->total_weight_kg = inputs->total_takeoff_weight_g/1000.0;
    outputs->wing_area = outputs->total_weight_kg*g/inputs->wing_loading;
    outputs->wing_span = std::sqrt(inputs->main_aspect_ratio*outputs->wing_area);
   
    outputs->MAC = (outputs->wing_area / outputs->wing_span) ;
    double numerator = 2.0*outputs->total_weight_kg*g;
    double denominator = rho*outputs->wing_area*inputs->max_lift_coeff;
    outputs->v_stall = std::sqrt(numerator/denominator);
    outputs->v_cruise = 1.30*outputs->v_stall;
    
   double aoaRad = inputs->angle_of_attack*(M_PI/180.0);
    if (inputs->angle_of_attack>15.0)
   {
    outputs->cl = (0.3 + 2.0*M_PI*aoaRad)*0.65;
   }
   else 
   {
    outputs->cl = (0.3 + 2.0*M_PI*aoaRad);
   }
    outputs->cl = std::max(-1.0, std::min(1.8, outputs->cl));
    //HORIZ
    numerator = (inputs->horiz_tail_vol_coeff* outputs->wing_area*outputs->MAC);
    outputs->horiz_wing_area = (numerator/inputs->total_moment_arm);
    outputs->horiz_wing_span = std::sqrt(inputs->horiz_aspect_ratio* outputs->horiz_wing_area);
    outputs->horiz_mac = outputs->horiz_wing_area/outputs->horiz_wing_span;
   numerator = 2*outputs->horiz_wing_area;
    denominator = (taper_ratio + 1)*outputs->horiz_wing_span;
  outputs->horiz_chord_root = numerator/denominator;
  outputs->horiz_chord_tip = taper_ratio*outputs->horiz_chord_root;
  outputs->elevator_breadth = (35*outputs->horiz_mac)/100;
  outputs->elevator_length = outputs->horiz_wing_span * 0.90;
  //VERT
  outputs->vert_wing_area = ((inputs->vert_tail_vol_coeff*outputs->wing_area*outputs->wing_span)/(inputs->total_moment_arm));
  outputs->vert_wing_span = std::sqrt(inputs->vert_aspect_ratio*outputs->vert_wing_area);
  numerator = 2*outputs->vert_wing_area;
  denominator = (taper_ratio+1)*( outputs->vert_wing_span);
  outputs->vert_chord_root = numerator/denominator;
  outputs->vert_chord_tip = taper_ratio*outputs->vert_chord_root;
outputs->vert_mac = outputs->vert_wing_area / outputs->vert_wing_span;
outputs->vert_height = (2*outputs->vert_wing_area) /(outputs->vert_chord_root+outputs->vert_chord_tip);
  //reynold
  outputs->reynolds_num = ((outputs->v_cruise*outputs->MAC)/Nu);
// lift 
outputs->lift = ((inputs->max_lift_coeff*rho*outputs->v_stall*outputs->v_stall*outputs->wing_area)/(2));
//Drag Force 
numerator = outputs->cl*outputs->cl;
denominator = M_PI*inputs->main_aspect_ratio;
outputs->cd = (CDO + numerator/denominator);
outputs->drag_force = 0.5*rho*outputs->v_stall*outputs->v_stall*outputs->wing_area*outputs->cd;
  }
  
void display_results( const UAVoutputs* outputs) {
    std::cout << "{"
              << "\"wing_span\":" << (outputs->wing_span) << "," 
              << "\"MAC\":" << (outputs->MAC ) << ","
              << "\"vert_cr\":" << (outputs->vert_chord_root ) << ","
              << "\"vert_ct\":" << (outputs->vert_chord_tip ) << ","
              << "\"vert_h\":" << (outputs->vert_height ) << ","
              
              
              << "\"total_weight_kg\":" << outputs->total_weight_kg << ","
              << "\"wing_area\":" << outputs->wing_area << ","
              << "\"v_stall\":" << outputs->v_stall << ","
              << "\"v_cruise\":" << outputs->v_cruise << ","
              << "\"cl\":" << outputs->cl << ","
              << "\"cd\":" << outputs->cd << ","
              << "\"drag_force\":" << outputs->drag_force << ","
              << "\"horiz_wing_area\":" << outputs->horiz_wing_area << ","
              << "\"horiz_wing_span\":" << outputs->horiz_wing_span << ","
              << "\"horiz_mac\":" << outputs->horiz_mac << ","
              << "\"horiz_chord_tip\":" << outputs->horiz_chord_tip << ","
              << "\"horiz_chord_root\":" << outputs->horiz_chord_root << ","
              << "\"elevator_breadth\":" << outputs->elevator_breadth << ","
              << "\"elevator_length\":" << outputs->elevator_length << ","
              << "\"vert_wing_area\":" << outputs->vert_wing_area << ","
              << "\"vert_wing_span\":" << outputs->vert_wing_span << ","
              << "\"vert_mac\":" << outputs->vert_mac << ","
              << "\"reynolds_num\":" << outputs->reynolds_num << ","
              << "\"lift\":" << outputs->lift
              << "}\n";
}
