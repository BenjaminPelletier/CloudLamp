#version 3.7;
global_settings { assumed_gamma 1.0 }

// +ki2 +kf3 +kfi0 +kff200

#declare camera_index = 0;

#if (camera_index = 0)
    // Perspective
    #declare camtheta = 720 * clock;
    #declare camdist = 40;
    camera {
       location  <camdist*cos(radians(camtheta)), 50*(1-mod(clock,1))-10, camdist*sin(radians(camtheta))>
       right     x*image_width/image_height
       direction <0, 0, 1.7>
       look_at   <0, 0, 0>
    }

#elseif (camera_index = 1)
    // Front
    camera {
        orthographic
        location <0,3,-50>
        look_at <0,3,0>
        angle 35
    }

#elseif (camera_index = 2)
    // Left
    camera {
        orthographic
        location <35,3,0>
        look_at <0,3,0>
        angle 35
    }

#elseif (camera_index = 3)
    // Top
    camera {
        orthographic
        location <0,40,0.1>
        look_at <0,3,0>
        angle 35
    }

#end

background { color rgb<1,1,1>*0.02 } 

light_source { <-15, 30, -25> color red 1 green 1 blue 1 }
light_source { < 15, 30, -25> color red 1 green 1 blue 1 }

// Slicer
#declare slicer = union {
    // Whole scene
    box { <-100, -30, -30>, <100, 30, 30> }
       
    // Side slices
    box { <-6, -30, -30>, <-5.9, 30, 30> } // A
    box { <-4, -30, -30>, <-3.9, 30, 30> } // B
    box { <-2, -30, -30>, <-1.9, 30, 30> } // C
    box { <0, -30, -30>, <0.1, 30, 30> } // D
    box { <2, -30, -30>, <2.1, 30, 30> } // E
    box { <4, -30, -30>, <4.1, 30, 30> } // F
    box { <6, -30, -30>, <6.1, 30, 30> } // G
    
    
    // Front slices
    box { <-30, -30, -5>, <30, 30, -4.9> } // H
    box { <-30, -30, -3>, <30, 30, -2.9> } // I
    box { <-30, -30, -1>, <30, 30, -0.9> } // J
    box { <-30, -30, 1>, <30, 30, 1.1> } // K
    box { <-30, -30, 3>, <30, 30, 3.1> } // L
    box { <-30, -30, 5>, <30, 30, 5.1> } // M
    
    // Belt slices
    box { <-30, 0, -30>, <30, 0.1, 30> } // N
    box { <-30, 4, -30>, <30, 4.1, 30> } // O
    
};

// Base
intersection {
    difference {
        box { <-6, -26.25, -4.5>, <6, -18.25, 4.5> }
        box { <-3.937, -25.5, -2.362>, <3.937, 0, 2.362> }
    }
    slicer
    pigment { color rgb<0.4, 0.4, 0.4> }
}

// Post
box { <4.125, -26.25, 4.375>, <5.125, 3.75, 5.375> pigment { color rgb<0.6, 0.6, 0.6> } }

// Platform

// Mounting bars
intersection {
    union {
        box { <-7, 4, -2.375>, <7, 4.75, -1.625> } // Front
        box { <-7, 4, 1.625>, <7, 4.75, 2.375> } // Back
        box { <-7, 4.75, -5.5>, <-6.25, 5.5, 5.5> } // Left
        box { <6.25, 4.75, -5.5>, <7, 5.5, 5.5> } // Right
    }
    slicer
    pigment { color rgb<0.5, 0, 0.7> }
}

// Reservoir
intersection {
    box { <-4, 0, -2.375>, <4, 3, 2.375> }
    slicer
    pigment { color rgb<0.9, 0.9, 0.9> }
}

// Markers
union {
    box { <-10, -3, -10>, <-9.9, -2.9, -9.9> }
    box { <10, -3, -10>, <9.9, -2.9, -9.9> }
    box { <-10, 10, -10>, <-9.9, 9.9, -9.9> }
    box { <10, 10, -10>, <9.9, 9.9, -9.9> }
    box { <-10, -3, 10>, <-9.9, -2.9, 9.9> }
    box { <10, -3, 10>, <9.9, -2.9, 9.9> }
    box { <-10, 10, 10>, <-9.9, 9.9, 9.9> }
    box { <10, 10, 10>, <9.9, 9.9, 9.9> }
    
    pigment { color rgb<1,1,0> }
}

#declare cloud_index = 3; //floor(clock);

// Cloud
#macro cloud_blob(thresh)
    blob {
        threshold thresh
        
        #if (cloud_index = 0)
        sphere { <-4.01,2.99,-2.51>, 3.75, 2.0 }
        sphere { <0.10,2.51,1.28>, 7.39, 2.0 }
        sphere { <-7.36,1.19,-2.46>, 7.79, 2.0 }
        sphere { <-6.62,3.34,-6.15>, 2.96, 2.0 }
        sphere { <4.27,3.47,2.63>, 7.40, 2.0 }
        sphere { <3.98,0.15,-0.97>, 3.22, 2.0 }
        sphere { <3.12,1.14,4.69>, 6.80, 2.0 }
        sphere { <6.53,4.54,5.95>, 1.97, 2.0 }
        sphere { <-1.68,4.27,-2.73>, 6.74, 2.0 }
        sphere { <-2.70,-0.39,-2.85>, 2.31, 2.0 }
        sphere { <-0.85,-0.56,-2.20>, 1.87, 2.0 }
        sphere { <-0.50,1.18,-2.08>, 2.53, 2.0 }
        sphere { <1.94,-0.76,-1.13>, 2.72, 2.0 }
        sphere { <0.72,-0.17,-2.48>, 0.97, 2.0 }
        sphere { <-6.73,6.40,2.34>, 6.35, 2.0 }
        sphere { <-6.76,2.70,5.63>, 7.50, 2.0 }
        sphere { <-4.63,-1.18,3.35>, 3.70, 2.0 }
        sphere { <-3.64,-0.65,2.38>, 1.96, 2.0 }
        sphere { <6.68,5.53,-2.95>, 2.51, 2.0 }
        sphere { <6.87,5.14,-2.67>, 5.80, 2.0 }
        sphere { <2.58,2.55,-1.81>, 5.06, 2.0 }
        
        
        #elseif (cloud_index = 1)
        sphere { <-0.23,4.33,2.32>, 2.82, 2.0 }
        sphere { <3.54,3.37,-0.26>, 6.92, 2.0 }
        sphere { <2.30,0.35,-0.40>, 7.60, 2.0 }
        sphere { <-4.77,3.14,1.42>, 4.90, 2.0 }
        sphere { <6.10,3.53,3.73>, 3.73, 2.0 }
        sphere { <6.04,3.54,-2.60>, 3.92, 2.0 }
        sphere { <4.39,1.65,2.39>, 4.87, 2.0 }
        sphere { <-4.87,2.85,-2.06>, 4.95, 2.0 }
        sphere { <-5.34,4.53,-2.91>, 4.12, 2.0 }
        sphere { <4.42,3.88,-0.26>, 7.26, 2.0 }
        sphere { <-4.28,2.14,-1.65>, 7.52, 2.0 }
        sphere { <-5.50,5.26,1.00>, 4.90, 2.0 }
        sphere { <6.27,3.27,-4.28>, 3.40, 2.0 }
        sphere { <3.36,1.28,1.35>, 7.05, 2.0 }
        sphere { <-7.01,5.32,3.88>, 2.10, 2.0 }
        sphere { <4.23,1.41,2.97>, 4.95, 2.0 }
        sphere { <-5.91,5.78,-4.22>, 3.64, 2.0 }
        sphere { <3.00,0.76,3.61>, 4.30, 2.0 }
        sphere { <4.63,4.77,3.04>, 4.95, 2.0 }
        sphere { <-5.64,4.72,-3.92>, 3.88, 2.0 }
        sphere { <4.85,0.42,3.75>, 3.60, 2.0 }
        sphere { <5.56,5.14,-3.36>, 5.06, 2.0 }
        sphere { <6.29,4.54,4.39>, 2.65, 2.0 }
        sphere { <-7.06,4.76,5.17>, 2.04, 2.0 }
        sphere { <-0.07,2.87,-1.66>, 5.52, 2.0 }
        sphere { <-0.84,4.74,2.63>, 2.55, 2.0 }
        sphere { <-3.28,-0.52,2.58>, 2.17, 2.0 }
        sphere { <-1.68,-0.38,2.82>, 2.49, 2.0 }
        sphere { <1.17,0.11,4.12>, 1.53, 2.0 }
        sphere { <-0.95,-0.57,4.68>, 2.39, 2.0 }
        
        #elseif (cloud_index = 2)
        sphere { <2.45,0.39,-0.39>, 4.50, 2.0 }
        sphere { <2.62,5.17,3.50>, 4.14, 2.0 }
        sphere { <-4.23,1.46,-1.57>, 7.11, 2.0 }
        sphere { <3.04,2.37,1.27>, 5.91, 2.0 }
        sphere { <4.02,3.36,-1.83>, 6.30, 2.0 }
        sphere { <-1.77,4.74,3.64>, 3.72, 2.0 }
        sphere { <3.70,2.08,2.66>, 5.62, 2.0 }
        sphere { <-0.69,4.07,-0.85>, 6.23, 2.0 }
        sphere { <-5.59,4.88,-0.80>, 3.72, 2.0 }
        sphere { <-2.65,3.01,0.83>, 5.58, 2.0 }
        sphere { <0.54,1.99,2.30>, 6.21, 2.0 }
        sphere { <5.27,4.09,0.19>, 4.83, 2.0 }
        sphere { <-4.30,3.58,1.05>, 5.14, 2.0 }
        sphere { <-5.04,2.36,1.20>, 6.08, 2.0 }
        sphere { <5.32,3.55,-1.51>, 5.34, 2.0 }
        sphere { <-4.35,3.82,-0.14>, 7.38, 2.0 }
        sphere { <5.40,2.55,3.05>, 5.08, 2.0 }
        sphere { <5.27,2.64,1.99>, 5.82, 2.0 }
        sphere { <4.52,3.44,2.06>, 7.04, 2.0 }
        sphere { <-5.23,4.19,-3.75>, 3.63, 2.0 }
        sphere { <-5.16,5.72,3.41>, 4.17, 2.0 }
        sphere { <-6.04,6.08,-4.00>, 3.53, 2.0 }
        sphere { <6.59,5.05,-4.99>, 1.73, 2.0 }
        sphere { <-6.11,4.31,3.62>, 4.09, 2.0 }
        sphere { <-5.75,4.75,-4.30>, 3.99, 2.0 }
        sphere { <7.18,4.83,5.63>, 0.64, 2.0 }
        sphere { <4.20,-0.45,4.69>, 1.54, 2.0 }
        sphere { <5.67,1.47,5.21>, 1.33, 2.0 }
        sphere { <4.67,0.42,4.94>, 1.96, 2.0 }
        sphere { <1.54,0.68,4.22>, 2.94, 2.0 }
        sphere { <3.32,0.73,3.96>, 2.64, 2.0 }
        sphere { <-0.98,0.17,1.76>, 4.76, 2.0 }
        sphere { <1.75,-0.76,-2.19>, 2.80, 2.0 }
        sphere { <0.21,-0.29,-2.83>, 1.67, 2.0 }
        
        #elseif (cloud_index = 3)
        sphere { <-4.04,5.40,3.07>, 3.83, 2.0 }
        sphere { <0.53,1.14,-1.55>, 5.18, 2.0 }
        sphere { <1.10,0.47,-2.93>, 4.45, 2.0 }
        sphere { <2.15,4.15,2.03>, 5.66, 2.0 }
        sphere { <5.21,5.16,4.43>, 2.24, 2.0 }
        sphere { <-3.23,0.97,1.56>, 6.07, 2.0 }
        sphere { <-0.16,1.29,2.88>, 5.55, 2.0 }
        sphere { <4.24,1.14,-4.14>, 4.31, 2.0 }
        sphere { <5.83,0.59,2.17>, 4.66, 2.0 }
        sphere { <2.80,4.19,-1.70>, 6.63, 2.0 }
        sphere { <6.97,3.83,-4.52>, 2.79, 2.0 }
        sphere { <4.60,3.06,1.65>, 4.36, 2.0 }
        sphere { <5.84,3.78,0.79>, 4.64, 2.0 }
        sphere { <-6.56,5.16,3.99>, 3.20, 2.0 }
        sphere { <-4.22,2.97,-0.59>, 6.81, 2.0 }
        sphere { <4.11,1.07,2.05>, 6.11, 2.0 }
        sphere { <4.94,1.63,4.67>, 2.27, 2.0 }
        sphere { <-5.70,4.60,-1.23>, 4.29, 2.0 }
        sphere { <6.93,3.69,3.29>, 2.30, 2.0 }
        sphere { <2.54,1.66,2.46>, 5.55, 2.0 }
        sphere { <4.04,0.88,3.64>, 4.33, 2.0 }
        sphere { <-4.91,4.05,-2.37>, 5.22, 2.0 }
        sphere { <-4.76,4.78,-3.59>, 4.75, 2.0 }
        sphere { <5.17,2.95,2.60>, 5.13, 2.0 }
        sphere { <6.17,4.04,2.97>, 3.88, 2.0 }
        sphere { <4.27,4.45,4.16>, 3.06, 2.0 }
        sphere { <-6.25,5.04,-4.52>, 3.17, 2.0 }
        sphere { <6.97,4.33,4.89>, 1.49, 2.0 }
        sphere { <-1.11,4.55,1.73>, 2.30, 2.0 }
        sphere { <-5.63,5.30,2.31>, 2.15, 2.0 }
        sphere { <-7.62,4.72,1.22>, 1.49, 2.0 }
        sphere { <6.45,4.40,-1.58>, 2.33, 2.0 }
        sphere { <-0.78,-0.44,-2.02>, 4.46, 2.0 }
        sphere { <-0.81,5.23,-2.32>, 1.57, 2.0 }
        
        #end
    }
#end

intersection {
    difference {
        cloud_blob(1.0)
        cloud_blob(1.1)
        box { <-6, -30, -2.75>, <6, 4.25, 2.75> }
    }
    slicer
    
    pigment { color rgb< 0.7,0,0.05> }
    finish {
        ambient 0.1
        diffuse 0.7
        phong 1
    }
}