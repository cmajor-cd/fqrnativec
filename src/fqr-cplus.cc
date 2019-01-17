/**
 * https://github.com/electron/electron/blob/v0.37.2/docs/tutorial/using-native-node-modules.md#using-native-node-modules
How to Install Native Modules

Three ways to install native modules:
==The Easy Way
The most straightforward way to rebuild native modules is via the electron-rebuild package, which handles the manual steps of downloading headers and building native modules:
-------
npm install --save-dev electron-rebuild
# Every time you run "npm install", run this
./node_modules/.bin/electron-rebuild

# On Windows if you have trouble, try:
.\node_modules\.bin\electron-rebuild.cmd

==The npm Way
You can also use npm to install modules. The steps are exactly the same with Node modules, except that you need to setup some environment variables:
---------
export npm_config_disturl=https://atom.io/download/atom-shell
export npm_config_target=0.33.1
export npm_config_arch=x64
export npm_config_runtime=electron
HOME=~/.electron-gyp npm install module-name

==The node-gyp Way
--------
To build Node modules with headers of Electron, you need to tell node-gyp where to download headers and which version to use:
$ cd /path-to-module/
$ HOME=~/.electron-gyp node-gyp rebuild --target=0.29.1 --arch=x64 --dist-url=https://atom.io/download/atom-shell
The HOME=~/.electron-gyp changes where to find development headers. The --target=0.29.1 is version of Electron. The --dist-url=... specifies where to download the headers. The --arch=x64 says the module is built for 64bit system.
*/
/** -----------------
 * How to create a npm module of source C/C++ by manually.
   1. create your npm module/create dir with c source code
   2. create binding.gyp
   {
    "targets": [
        {
          "target_name": "fqrnativec",
          "sources": [ "./src/native/fqr-cplus.cc" ]
          # "include_dirs": [
          #   "<!(node -e \"require('nan')\")"
          # ]
        }
      ]
    }
    3. create package.json file
    {
        "name": "fqrnativec",
        "version": "1.0.0",
        "description": "fqrNativeC",
        "main": "main.js",
        "scripts": {
          "compile": "node-gyp rebuild -target=1.6.11 -arch=x64 -dist-url=https://atom.io/download/atom-shell"
        },
        "author": "YangYutong",
        "license": "MIT",
        "gypfile": true
    }
    4. create npm's index.js (or main.js)
      module.exports = require('./fqrnativec.node');
    5. compile c/c++ source code
      npm run compile ==> fqrnativec.node
    6. create ./node_modules/fqrnativec dir.
    7. copy files into ./node_modules/fqrnativec
      package.json + index.js (or main.js) + fqrnativec.node
    8. add lib name into application's package.json
      "dependencies": {
          "fqrnativec": "^1.0.0"
        }
    9. build to get exe
*/
/** How to modify and debug by npm install mode ---------------------
 * 1. import from: npm install fqrnativec
 * 2. modify the files in ./node_moduls/fqrnativec/src/xxxx.c
 * 3. delete the directory ./node_moduls/fqrnativec/build
 * 4. compile it by: .\node_modules\.bin\electron-rebuild.cmd
 * 5. merge changed code into master branch of fqrnativec, then update npm
*/

#include <node.h>
#include <stdio.h>
#include <stdlib.h>

using namespace v8;

/**
 * struction of the data file per line.
*/
struct element_struc
{
  double longitude; //d.7:float=>6~7 numbers after '.' / double=>15~16 numbers after '.'
  double latitude;  //d.7
  int satellite_number; // 1
  float sat_altitude; //0.01m
  float sat_speed;    //0.1km/h
  float x_acc;  //0.09ug / 0.00009 g
  float y_acc;  //0.09ug
  float z_acc;  //0.09ug
  float roll;   //0.1 deg
  float pitch;  //0.1 deg
  float heading;//0.1 deg
  float roll_angspd;    //0.01deg/sec
  float pitch_angspd;   //0.01deg/sec
  float heading_angspd; //0.01deg/sec
  float up_speed;    //<0.1m/s
  float down_speed;  //<0.1m/s
};

/**
 * 2018-8-28: confirm the max/min
*/
#define MAX_longitude (180)
#define MIN_longitude (-180)
#define MAX_latitude (90)
#define MIN_latitude (-90)
#define MAX_satellite_number (100)
#define MIN_satellite_number (0)
#define MAX_sat_altitude (20000)
#define MIN_sat_altitude (0)
#define MAX_sat_speed (2000)
#define MIN_sat_speed (0)
#define MAX_x_acc (100)
#define MIN_x_acc (-100)
#define MAX_y_acc (100)
#define MIN_y_acc (-100)
#define MAX_z_acc (100)
#define MIN_z_acc (-100)
#define MAX_roll (180)
#define MIN_roll (-180)
#define MAX_pitch (90)
#define MIN_pitch (-90)
#define MAX_heading (360)
#define MIN_heading (0)
#define MAX_roll_angspd (180)
#define MIN_roll_angspd (-180)
#define MAX_pitch_angspd (180)
#define MIN_pitch_angspd (-180)
#define MAX_heading_angspd (180)
#define MIN_heading_angspd (-180)
#define MAX_up_speed (500)
#define MIN_up_speed (0)
#define MAX_down_speed (500)
#define MIN_down_speed (0)

/**
 * check the element_struc date ,if overflow then set to 0
*/
void checkTheDat(element_struc *p_input){
  p_input->longitude = p_input->longitude >= MAX_longitude? MAX_longitude:p_input->longitude;
  p_input->longitude = p_input->longitude <= MIN_longitude? MIN_longitude:p_input->longitude;

  p_input->latitude = p_input->latitude >= MAX_latitude? MAX_latitude:p_input->latitude;
  p_input->latitude = p_input->latitude <= MIN_latitude? MIN_latitude:p_input->latitude;

  p_input->satellite_number = p_input->satellite_number >= MAX_satellite_number? MAX_satellite_number:p_input->satellite_number;
  p_input->satellite_number = p_input->satellite_number <= MIN_satellite_number? MIN_satellite_number:p_input->satellite_number;

  p_input->sat_altitude = p_input->sat_altitude >= MAX_sat_altitude? MAX_sat_altitude:p_input->sat_altitude;
  p_input->sat_altitude = p_input->sat_altitude <= MIN_sat_altitude? MIN_sat_altitude:p_input->sat_altitude;

  p_input->sat_speed = p_input->sat_speed >= MAX_sat_speed? MAX_sat_speed:p_input->sat_speed;
  p_input->sat_speed = p_input->sat_speed <= MIN_sat_speed? MIN_sat_speed:p_input->sat_speed;

  p_input->x_acc = p_input->x_acc >= MAX_x_acc? MAX_x_acc:p_input->x_acc;
  p_input->x_acc = p_input->x_acc <= MIN_x_acc? MIN_x_acc:p_input->x_acc;

  p_input->y_acc = p_input->y_acc >= MAX_y_acc? MAX_y_acc:p_input->y_acc;
  p_input->y_acc = p_input->y_acc <= MIN_y_acc? MIN_y_acc:p_input->y_acc;

  p_input->z_acc = p_input->z_acc >= MAX_z_acc? MAX_z_acc:p_input->z_acc;
  p_input->z_acc = p_input->z_acc <= MIN_z_acc? MIN_z_acc:p_input->z_acc;

  p_input->roll = p_input->roll >= MAX_roll? MAX_roll:p_input->roll;
  p_input->roll = p_input->roll <= MIN_roll? MIN_roll:p_input->roll;

  p_input->pitch = p_input->pitch >= MAX_pitch? MAX_pitch:p_input->pitch;
  p_input->pitch = p_input->pitch <= MIN_pitch? MIN_pitch:p_input->pitch;

  p_input->heading = p_input->heading >= MAX_heading? MAX_heading:p_input->heading;
  p_input->heading = p_input->heading <= MIN_heading? MIN_heading:p_input->heading;

  p_input->roll_angspd = p_input->roll_angspd >= MAX_roll_angspd? MAX_roll_angspd:p_input->roll_angspd;
  p_input->roll_angspd = p_input->roll_angspd <= MIN_roll_angspd? MIN_roll_angspd:p_input->roll_angspd;

  p_input->pitch_angspd = p_input->pitch_angspd >= MAX_pitch_angspd? MAX_pitch_angspd:p_input->pitch_angspd;
  p_input->pitch_angspd = p_input->pitch_angspd <= MIN_pitch_angspd? MIN_pitch_angspd:p_input->pitch_angspd;

  p_input->heading_angspd = p_input->heading_angspd >= MAX_heading_angspd? MAX_heading_angspd:p_input->heading_angspd;
  p_input->heading_angspd = p_input->heading_angspd <= MIN_heading_angspd? MIN_heading_angspd:p_input->heading_angspd;

  p_input->up_speed = p_input->up_speed >= MAX_up_speed? MAX_up_speed:p_input->up_speed;
  p_input->up_speed = p_input->up_speed <= MIN_up_speed? MIN_up_speed:p_input->up_speed;

  p_input->down_speed = p_input->down_speed >= MAX_down_speed? MAX_down_speed:p_input->down_speed;
  p_input->down_speed = p_input->down_speed <= MIN_down_speed? MIN_down_speed:p_input->down_speed;

}

/** ------------------
 * read the data file by line number.
*/
void readFileDataByLine(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  // create rc object
  Local<Object> rcObj = Object::New(isolate);
  Local<Number> rcSucc = Uint32::New(isolate, 0); // 0: succ
  Local<Number> rcFail = Uint32::New(isolate, 1); // 1: fail

  // Check the number of arguments passed.
  if (args.Length() < 2) {
    // Throw an Error that is passed back to JavaScript
    // isolate->ThrowException(Exception::TypeError(
    //     String::NewFromUtf8(isolate, "Wrong number of arguments")));
    rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
    rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Wrong number of arguments"));
    return;
  }

  // Check the argument types
  if (!args[0]->IsString() || !args[1]->IsNumber()) {
    rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
    rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Wrong type of arguments"));
    return;
  }
  v8::String::Utf8Value str(args[0]->ToString());
  const char *c_fullpath = *str;
	long line = args[1]->NumberValue();

  //-- process file 
    element_struc line_dat;
    int e_size = 0, f_offset = 0, r_len = 0;
    FILE *fp_r = 0;
    fp_r = fopen(c_fullpath, "rb");
    if( 0 != fp_r){
      // rewind(fp_r);
      e_size = sizeof(element_struc);
      memset(&line_dat, 0, e_size);
      f_offset = line * e_size;
      int seeked = fseek(fp_r, f_offset, SEEK_SET);
      if( 0 == seeked ){
        r_len = fread(&line_dat, e_size, 1, fp_r);
        if( 1 == r_len){
          // check the set the data
          checkTheDat(&line_dat);
          // v8::TryCatch try_catch; // try{}
          char str_a[20]; 
          sprintf(str_a, "%.7f", line_dat.longitude);
          rcObj->Set(String::NewFromUtf8(isolate, "longitude"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.7f", line_dat.latitude);
          rcObj->Set(String::NewFromUtf8(isolate, "latitude"), String::NewFromUtf8(isolate, str_a));
          rcObj->Set(String::NewFromUtf8(isolate, "satellite_number"), Uint32::New(isolate, line_dat.satellite_number));
          sprintf(str_a, "%.2f", line_dat.sat_altitude);
          rcObj->Set(String::NewFromUtf8(isolate, "sat_altitude"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.1f", line_dat.sat_speed);
          rcObj->Set(String::NewFromUtf8(isolate, "sat_speed"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.5f", line_dat.x_acc);
          rcObj->Set(String::NewFromUtf8(isolate, "x_acc"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.5f", line_dat.y_acc);
          rcObj->Set(String::NewFromUtf8(isolate, "y_acc"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.5f", line_dat.z_acc);
          rcObj->Set(String::NewFromUtf8(isolate, "z_acc"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.1f", line_dat.roll);
          rcObj->Set(String::NewFromUtf8(isolate, "roll"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.1f", line_dat.pitch);
          rcObj->Set(String::NewFromUtf8(isolate, "pitch"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.1f", line_dat.heading);
          rcObj->Set(String::NewFromUtf8(isolate, "heading"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.2f", line_dat.roll_angspd);
          rcObj->Set(String::NewFromUtf8(isolate, "roll_angspd"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.2f", line_dat.pitch_angspd);
          rcObj->Set(String::NewFromUtf8(isolate, "pitch_angspd"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.2f", line_dat.heading_angspd);
          rcObj->Set(String::NewFromUtf8(isolate, "heading_angspd"), String::NewFromUtf8(isolate, str_a));
          //
          sprintf(str_a, "%.1f", line_dat.up_speed);
          rcObj->Set(String::NewFromUtf8(isolate, "up_speed"), String::NewFromUtf8(isolate, str_a));
          sprintf(str_a, "%.1f", line_dat.down_speed);
          rcObj->Set(String::NewFromUtf8(isolate, "down_speed"), String::NewFromUtf8(isolate, str_a));
          //
          rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcSucc);
          // if(try_catch.HasCaught())
          // {
          //   rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
          //   rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Fail to fill V8 element!"));
          // }
        }
        else{
          rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
          rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Fail to fread file!"));
        }

      }
      else{
        rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
        rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Fail to fseek file!"));
      }
    }
    else{
      rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
      rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Fail to open file!"));
    }
  fclose(fp_r);

  args.GetReturnValue().Set(rcObj);
}
/**-------------
 * get size of the whole file
*/
int get_file_size(FILE * file_handle)
{
  //get and save the currect fseek.
  unsigned int current_read_position=ftell( file_handle );
  int file_size;
  fseek( file_handle,0,SEEK_END );
  //get the file's size
  file_size=ftell( file_handle );
  //restore the fseek.
  fseek( file_handle,current_read_position,SEEK_SET );

  return file_size;
}
/** --------------------
 * read the outlook tracking data file by step.
 * void readFileLocationOutlookDataByStep(string filePath, int maxDatLines, int step)
 * @input: 
 *   filePath: full path of the file.
 *   maxDatLines: max data lines, if maxDatLines > maxDatFileLines => use maxDatFileLines, or use maxDatLines
 *   step: step of lines, when read the data.
 * @output:data array
*/
void readFileLocationOutlookDataByStep(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  // create rc object
  Local<Object> rcObj = Object::New(isolate);
  Local<Array>  rcArray = Array::New(isolate);
  Local<Number> rcSucc = Uint32::New(isolate, 0); // 0: succ
  Local<Number> rcFail = Uint32::New(isolate, 1); // 1: fail
  // init rc => succ
  rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcSucc);

  // Check the number of arguments passed.
  if (args.Length() != 3) {
    // Throw an Error that is passed back to JavaScript
    // isolate->ThrowException(Exception::TypeError(
    //     String::NewFromUtf8(isolate, "Wrong number of arguments")));
    rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
    rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Wrong number of arguments"));
    return;
  }

  // Check the argument types
  if (!args[0]->IsString() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
    rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
    rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Wrong type of arguments"));
    return;
  }
  v8::String::Utf8Value str(args[0]->ToString());
  const char *c_fullpath = *str;
	int maxDatLines = args[1]->NumberValue();
  int step = args[2]->NumberValue();

  //-- process file 
    element_struc line_dat;
    int e_size = 0, f_offset = 0, r_len = 0;
    FILE *fp_r = 0;
    fp_r = fopen(c_fullpath, "rb");
    if( 0 != fp_r){
      // rewind(fp_r);
      e_size = sizeof(element_struc);
      memset(&line_dat, 0, e_size);

      int max_lines = (int) get_file_size(fp_r)/e_size;
      if(maxDatLines < max_lines){
        max_lines = maxDatLines;
      }
      // read data, by line by step.
      int idx = 0;
      for(int line = 1; line < max_lines; line = line + step, idx++){
        f_offset = line * e_size;
        int seeked = fseek(fp_r, f_offset, SEEK_SET);
        if( 0 == seeked ){
          r_len = fread(&line_dat, e_size, 1, fp_r);
          if( 1 == r_len){
            // check the set the data
            checkTheDat(&line_dat);
            // v8::TryCatch try_catch; // try{}
            Local<Array> lineArray = Array::New(isolate);
            char str_a[20]; 
            sprintf(str_a, "%.7f", line_dat.longitude);
            lineArray->Set(0, String::NewFromUtf8(isolate, str_a));
            sprintf(str_a, "%.7f", line_dat.latitude);
            lineArray->Set(1, String::NewFromUtf8(isolate, str_a));
            // create rcArray
            rcArray->Set(idx, lineArray);
          }
          else{
            rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
            rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Fail to fread file!"));
            break;
          }
        }
        else{
          rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
          rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Fail to fseek file!"));
          break;
        }
      } // read data, for(){}
      // fill the 'location': rcArray
      rcObj->Set(String::NewFromUtf8(isolate, "location"), rcArray);
    } // open file {}
    else{
      rcObj->Set(String::NewFromUtf8(isolate, "rc"), rcFail);
      rcObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Fail to open file!"));
    }
  fclose(fp_r);

  args.GetReturnValue().Set(rcObj);
}

// void closeFile(const FunctionCallbackInfo<Value>& args) {
//     if(0 != fp_r){
//       fclose(fp_r);
//     }
// }
//
void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "readFileDataByLine", readFileDataByLine);
  NODE_SET_METHOD(exports, "readFileLocationOutlookDataByStep", readFileLocationOutlookDataByStep);
  // NODE_SET_METHOD(exports, "closeFile", closeFile);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

