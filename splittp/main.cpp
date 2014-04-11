//
//  main.cpp
//  splittp
//
//  Created by duanhouhai on 14-4-11.
//  Copyright (c) 2014年 duanhouhai. All rights reserved.
//

#include <iostream>
#include <string>
#include <Magick++.h>

using namespace Magick;

int main(int argc, char ** argv)
{

    InitializeMagick(*argv);
    
    try {
        
        
        
        Image packImage("0.png");
        
        float fromX=44.0f;
        float fromY=184;
        float currentWidth=38,currentHeight=89;
        float offsetX=-5,offsetY=1;
        float orignalWidth=62,orignalHeight=91;
        
        float colorX=(orignalWidth-currentWidth)/2+offsetX;
        float colorY=(orignalHeight-currentHeight)/2-offsetY;
        
        //get current image
        
        Image currentImage(packImage);
        
        currentImage.chop(Geometry(fromX,fromY));
        currentImage.crop(Geometry(currentWidth,currentHeight));
        
//        currentImage.write("00.png");
        
        Image orignalImage(Geometry(orignalWidth,orignalHeight),Color());
        
        DrawableCompositeImage img(colorX,colorY,currentImage);

        orignalImage.draw(img);
        
        orignalImage.write("00_1.png");
        
    } catch (Magick::Exception e) {
        printf("ex:%s\n",e.what());

    }
    
    
//    test.write("2.png");
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

