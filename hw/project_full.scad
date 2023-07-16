$fn=50;
stemma_x=4.25; stemma_y=6; stemma_z=2.92;
system_width=70;system_depth=25;enclosure_thickness=1;support_thickness=2.5; support_height=5;
distance_between_sensors = 55;
tof_hole_size=2.46;
tof_holes_distance_x = 20.36; tof_holes_distance_y = 12.71; tof_holes_offset_x = 1.36; tof_holes_offset_y = 1.36;
sensor_x=2.33;sensor_y=4.39;sensor_z=1;
tof_x=25.62;tof_y=17.8;tof_board_z=1.7;
flappytator_diameter = 3; flappytator_extra=10; flappytator_offset=0.5; 
flappytator_head_length=10;flappytator_head_width=1;flappytator_head_depth=2;board_shift_on_tator=1;
cable_hole = 8;

module stemma() {
    color("grey") cube([stemma_x, stemma_y, stemma_z]);
}

module sensor() {
    color("purple") cube([sensor_x, sensor_y, sensor_z]);
}

module tof_board() {
    color("green") cube([tof_x, tof_y, tof_board_z]);
    // Centered components top
    translate([0, tof_y/2, tof_board_z]) {
        // Ports
        translate([0, -stemma_y/2,0]) {
            stemma();
            translate([tof_x - stemma_x,0,0]) stemma();
        }
        // Sensor
        translate([tof_x/2-sensor_x/2, -sensor_y/2,0]) {
            sensor();
        }
    }   
}

module tof_holes_v(height) {
            cylinder(h=height,d=tof_hole_size, center=true);
            translate([0,tof_holes_distance_y,0]) cylinder(h=height,d=tof_hole_size, center=true);
}
module tof_holes(height_board=tof_board_z) {
    translate([tof_holes_offset_x+tof_hole_size/2, tof_holes_offset_y+tof_hole_size/2, height_board/2]) {
        tof_holes_v(height=tof_board_z*2);
        translate([tof_holes_distance_x,0,0]) tof_holes_v(height=height_board*2);
    }
}

module tof() {
    difference() { tof_board(); tof_holes(); }
}

flappytator_length=tof_y+flappytator_extra;
module tator() {
    color("red") union() {
        translate([-flappytator_offset,-flappytator_extra/2,flappytator_diameter/2]) { 
            rotate([-90,0,0]) {
                cylinder(h=flappytator_length, d=flappytator_diameter);
                translate([-flappytator_head_length*0,-flappytator_head_width/2,flappytator_length-flappytator_head_depth]) cube([flappytator_head_length, flappytator_head_width, flappytator_head_depth]);
            }
        }
    }
}

module flappy() {
     color("orange") difference() { 
        translate([0,0,0]) { cube([tof_x, tof_y, flappytator_diameter]); }
        tof_holes(flappytator_diameter);
        tator();
        
        translate([tof_x/4,tof_y/4,-flappytator_diameter/2]) minkowski() {
                cube([tof_x/2, tof_y/2, flappytator_diameter*2]);
                sphere(2);
            }
    }
}

module flappytator(print_mode) {
    if (print_mode) {
        translate([0,0,flappytator_length-flappytator_extra+flappytator_head_depth]) rotate([-90,0,0]) tator();
        translate([0,5,-flappytator_diameter]) flappy();
    } else {
        translate([0,0, -flappytator_diameter]) { 
            tator();
            translate([0,-board_shift_on_tator,0]) flappy();
        }
    }
}

// Used to offset to the center of the cylinder
module toftator(show_boards=false, print_mode=true) {
    if (print_mode) {
        flappytator(print_mode);
    } else {
        translate([flappytator_offset,0,flappytator_diameter/2]) {
            if (show_boards)
                translate([0,-board_shift_on_tator,0]) tof(); 
            flappytator();
        }
    }
}


shift_sensors_x=(system_width-distance_between_sensors)/2;

module enclosure() {
    difference() {
        union() {
            cube([system_width, system_depth, enclosure_thickness]);
            cube([system_width, support_thickness, support_height]);
            translate([0,system_depth-support_thickness,0]) cube([system_width, support_thickness, support_height]);
        }
        // holes for tators
        translate([shift_sensors_x,-1,flappytator_diameter]) rotate([-90,0,0]) cylinder(h=flappytator_length+5, d=flappytator_diameter);
        translate([shift_sensors_x+distance_between_sensors,-1,flappytator_diameter]) rotate([-90,0,0]) cylinder(h=flappytator_length+5, d=flappytator_diameter);
        // holes for cables
        translate([shift_sensors_x-cable_hole/3,system_depth/2,-enclosure_thickness/2]) {
            cylinder(d=cable_hole,h=enclosure_thickness*2);
            translate([distance_between_sensors+cable_hole*2/3,0,0]) cylinder(d=cable_hole,h=enclosure_thickness*2);
        }
        translate([system_width/4, system_depth/4, -enclosure_thickness/2]) { 
            minkowski() {
                cube([system_width/2, system_depth/2, enclosure_thickness*2]);
                sphere(3);
            }
        }
        
        translate([support_thickness,support_thickness*2.5 /* bad */,-enclosure_thickness/2]) { 
            cylinder(d=tof_hole_size, h=enclosure_thickness*2);
            translate([0,system_depth/2,0]) cylinder(d=tof_hole_size, h=enclosure_thickness*2);
            translate([system_width-support_thickness*2,0,0]) {
                cylinder(d=tof_hole_size, h=enclosure_thickness*2);
                translate([0,system_depth/2,0]) cylinder(d=tof_hole_size, h=enclosure_thickness*2);
            }
        }
    }
}

module run(show_boards=false, print_mode=true) {
    translate([0,(print_mode ? 30 : 0),0]) enclosure();

    translate([shift_sensors_x,flappytator_extra/2,flappytator_diameter]) {
        rotate([0,-angle,0]) { toftator(show_boards, print_mode);  }
        translate([distance_between_sensors,0,0]) rotate([0,angle,0]) { mirror([1,0,0]) toftator(show_boards, print_mode); }
    }
}

show_boards = true;
print_mode = true;
angle= print_mode ? 0 : $t*90;

run(show_boards=show_boards, print_mode=print_mode);
