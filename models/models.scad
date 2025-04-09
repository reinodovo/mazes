include <../../ktane-3d-models/variables.scad>;
use <../../ktane-3d-models/bomb_base.scad>;

$fn = $preview ? 10 : 50;

tolerance = 0.3;

maze_distance = 10;
maze_size = 60;
maze_lip = 4;
maze_lip_width = 10;
maze_lip_thickness = 1;

maze_led_gap = 10;
maze_led_count = 6;
maze_led_radius = 2.5;
maze_led_tolerance = 0.2;

button_height = 6;
button_base = 10;
button_height_below = 6.1;
button_height_above = 1.8;
button_maze_distance = 1.8;

module button(tolerance = 0, height_tolerance = 0.2) {
    height_below = button_height_below - wall_thickness - height_tolerance;
    button_base_tol = button_base + tolerance;
    button_height_tol = button_height + tolerance;
    linear_extrude(button_height_above + wall_thickness) polygon([[-button_base_tol / 2, -button_height_tol / 2], [button_base_tol / 2, -button_height_tol / 2], [0, button_height_tol / 2]]);
    translate([0, 0, -height_below / 2]) cube([button_base_tol + 6, button_height_tol, height_below], center = true);
}

module maze_cover() {
    difference() {
        union() {
            translate([0, 0, wall_thickness / 2]) cube([maze_size - tolerance, maze_size - tolerance, wall_thickness], center = true);
            for (i = [-1:2:1]) translate([0, i * ((maze_size - tolerance) / 2 - maze_lip_width / 2), wall_thickness + maze_lip_thickness / 2]) cube([maze_size + 2 * maze_lip, maze_lip_width, maze_lip_thickness], center = true);
        }
        translate([- 2 * maze_led_gap - maze_led_gap / 2, - 2 * maze_led_gap - maze_led_gap / 2]) for (i = [0:maze_led_count - 1]) {
            for (j = [0:maze_led_count - 1]) {
                translate([i * maze_led_gap, j * maze_led_gap, 0]) cylinder(10, maze_led_radius + maze_led_tolerance, maze_led_radius + maze_led_tolerance, center = true);
            }
        }
    }
}

module top() {
    difference() {
        bomb_module_top(height_above_pcb = 10.6, module_type = 1, ignore_pillars = [1, 2]);
        translate([maze_distance + maze_size / 2, maze_distance + maze_size / 2]) {
            cube([maze_size, maze_size, 10], center = true);
            translate([0, 0, -wall_thickness]) for (i = [0:3]) {
                rotate([0, 0, i * 90]) translate([0, maze_size / 2 + button_height / 2 + button_maze_distance, 0]) button(tolerance = 0.2);
            }
        }
    }
}

module bottom() {
    bomb_module_bottom(height_above_pcb = 10.6, ignore_pillars = [1], skinny_pillars = true);
}

top();
bottom();
!maze_cover();
button(tolerance = -0.2, height_tolerance = 0.4);