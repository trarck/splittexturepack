//
//  main.cpp
//  splittp
//
//  Created by duanhouhai on 14-4-11.
//  Copyright (c) 2014年 duanhouhai. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <Magick++.h>
#include "Plist.h"
#include "Geometry.h"

using namespace Magick;

bool createDirectory(const char *path)
{
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    
//    printf("r:%d,err:%d\n",ret,errno);
    
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
}

bool createDirectorys(const char *path)
{
    
    size_t pre=0,pos;
    std::string dir;
    int mdret;
    
    std::string s=path;
    
    if(s[s.size()-1]!='/'){
        // force trailing / so we can handle everything in loop
        s+='/';
    }
    
    while((pos=s.find_first_of('/',pre))!=std::string::npos){
        dir=s.substr(0,pos++);
        pre=pos;
        if(dir.size()==0) continue; // if leading / first time is 0 length
        if((mdret=createDirectory(dir.c_str())) && errno!=EEXIST){
            return mdret;
        }
    }
    return mdret;
}

void parseFrame(Image* texture,Json::Value& frameData,const std::string& outFile)
{
    Rect frame=RectFromString(frameData["frame"].asCString());
    Point offset=PointFromString(frameData["offset"].asCString());
    Size sourceSize=SizeFromString(frameData["sourceSize"].asCString());

    bool rotated=frameData["rotated"].asBool();
    
    float colorX=(sourceSize.width-frame.width)/2+offset.x;
    float colorY=(sourceSize.height-frame.height)/2-offset.y;
    
    //get current image
    
    Image currentImage(*texture);
    currentImage.quality(100);
//    currentImage.type(TrueColorMatteType);
    
    currentImage.chop(Geometry(frame.x,frame.y));
    if (rotated) {
        currentImage.crop(Geometry(frame.height,frame.width));
        currentImage.rotate(-90);
    }else{
        currentImage.crop(Geometry(frame.width,frame.height));
    }
    
    Image orignalImage(Geometry(sourceSize.width,sourceSize.height),Color());
    
    orignalImage.quality(100);
    
//    orignalImage.type(TrueColorMatteType);
    
    DrawableCompositeImage img(colorX,colorY,currentImage);
    
    orignalImage.draw(img);

    //fix png convert.force out png32
    std::string png32File = std::string("png32:")+outFile;
    
    orignalImage.write(png32File);

}

int main(int argc, char ** argv)
{

    InitializeMagick(*argv);
    
    try {
        
        if (argc>1) {
            
            Plist plist;
            
            Json::Value data= plist.getDataFromFile(argv[1]);
            
            std::string outDir=argv[2];

            if (outDir[outDir.length()-1]!='/') {
                outDir+="/";
            }
            
            createDirectorys(outDir.c_str());
            
            Json::Value metadata=data["metadata"];
            
            std::string textureFileName="";
            
            if (argc==4) {
                textureFileName=argv[3];
            }else{
                textureFileName=metadata["textureFileName"].asString();
                
                std::string plistFile=argv[1];
                size_t pos=plistFile.find_last_of("/");
                if(pos!=std::string::npos){
                    std::string plistPath=plistFile.substr(0,pos);
                    textureFileName=plistPath+"/"+textureFileName;
                }
            }
            
            if (textureFileName!="") {
                Image textureImage(textureFileName);
                
                Json::Value frames=data["frames"];
                
                Json::Value::Members members=frames.getMemberNames();
                for (Json::Value::Members::iterator iter=members.begin(); iter!=members.end(); ++iter) {
                    
                    //check file have dir
                    size_t pos=(*iter).find_last_of("/");
                    if (pos!=std::string::npos) {
                        //have path
                        createDirectorys((outDir+(*iter).substr(0,pos)).c_str());
                    }
                    
                    std::string file=outDir+(*iter);

                    
                    parseFrame(&textureImage, frames[*iter], file);
                }

            }else{
                printf("texture file not exits");
            }
        }else{
            printf("splittp: plistFile outDir textureFile \n");
        }
        
    } catch (Magick::Exception e) {
        printf("ex:%s\n",e.what());

    }
    
    
//    test.write("2.png");
    
    // insert code here...
//    std::cout << "Hello, World!\n";
    return 0;
}

