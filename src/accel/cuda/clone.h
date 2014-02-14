/**
 * @file clone.h
 * @brief Routines to copy Material and Track objects to the GPU from CPU.
 * @author May 30, 2013
 * @author William Boyd, MIT, Course 22 (wboyd@mit.edu)
 */


#include "../DeviceMaterial.h"
#include "../DeviceTrack.h"

void clone_material_on_gpu(Material* material_h, dev_material* material_d);
void clone_track_on_gpu(Track* track_h, dev_track* track_d);
