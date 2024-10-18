import processing.serial.*;
import java.util.Arrays;

import ch.bildspur.postfx.builder.*;
import ch.bildspur.postfx.pass.*;
import ch.bildspur.postfx.*;

boolean debug = true;

PImage house;
PShape file;
PShape aichi;

Serial myPort;  // Create object from Serial class

PostFX fx;

ArrayList<PVector> leds = new ArrayList<PVector>();
byte[] colors = new byte[1350];

String header = "\nHEADER >>\n";
byte[] headerBytes = header.getBytes();

byte[] serialData = new byte[2000];
byte[] headerData = new byte[headerBytes.length];

int bytesMissing = 0; 
boolean drawFrame = false;

float spacing = 2.28;  // Specific distance between points

String structData = "structData.txt";
StringList allData = new StringList();

PVector xLimits = new PVector(220, 1200);
PVector yLimits = new PVector(140, 580);

Boolean outputData = false;

int totalLED = 0;


void setup() {
  size(1402, 631, P3D);
  
  printArray(Serial.list());
  
  String portName = Serial.list()[Serial.list().length-1];
  myPort = new Serial(this, portName, 115200);  
  
  
  fx = new PostFX(this); 
  fx.preload(BloomPass.class);
  // The file "bot1.svg" must be in the data folder
  // of the current sketch to load successfully
  house = loadImage("house.jpg");
  file = loadShape("house.svg");
  colorMode(RGB, 255, 255, 255);
  
  //println(file.getChildCount());
  
  image(house, 0, 0);
  allData.append("header");
  
  for(int j = 0; j < file.getChildCount(); j++)
  { 
    aichi = file.getChild(j);
    //println(aichi.getVertexCount());
    
    PVector[] vertices = new PVector[aichi.getVertexCount()];
    ArrayList<PVector> equallySpacedPoints = new ArrayList<PVector>();
    
    for(int i = 0; i < aichi.getVertexCount(); i++)
      vertices[i] = aichi.getVertex(i);
    
    equallySpacedPoints = getEquallySpacedPoints(vertices, spacing); 
    
    // Draw the shape
    /*
    beginShape();
    stroke(0);
    noFill();
    for (PVector v : vertices) {
      vertex(v.x, v.y);
    }
    endShape();
    */
    
    // Draw the points
    int sectionLED = 0;
    
    //allData = "tweenData ledData[ = {
      
    
    fill((255 / file.getChildCount()) * j, 255, 255);
    for (int i = 0; i < equallySpacedPoints.size(); i++)
    {
      PVector p = equallySpacedPoints.get(i);
      leds.add(p);
      
      PVector n = NormalizePoint(p);
      String ledData = "{" + j + ", " + sectionLED + ", " + n.x + ", " + n.y + "}";
      if(j != file.getChildCount()-1 || i != equallySpacedPoints.size()-1)
        ledData += ",";
      
      //println(ledData);
      allData.append(ledData);      
      //allData += ledData;
      sectionLED++;
      totalLED++;
      ellipse(p.x, p.y, 5, 5);
    }
  }
  
  println("Total LED: " + totalLED);
  
  colors = new byte[totalLED*3];
  
  allData.set(0, "tweenData ledData[" + totalLED + "] = {");
  allData.append(";");
  
  //println(totalLED);
  if(outputData);
    saveStrings(structData, allData.toArray());
  
  
} 

void draw(){
  if(myPort==null) return;
  //image(house, 0, 0);
  
  //shape(aichi, 0, 0);
  
  
  if(myPort.available() > 0)
  {
    int bytesAvailable = myPort.available();
    
    //if(bytesMissing == 0)
    if(bytesAvailable == 1363)
    {
      
      int readStart = 0;
      println("myPort: " + myPort.available());
      
      myPort.readBytes(serialData);
      arrayCopy(serialData, readStart, headerData, 0, headerBytes.length);
      
      
      
      //while(!Arrays.equals(headerData, headerBytes))
      //{
      //  if(readStart < (serialData.length - headerBytes.length))
      //    return;
      //  readStart++;
      //  arrayCopy(serialData, readStart, headerData, 0, headerBytes.length);
      //}
      
      
      
      if(Arrays.equals(headerData, headerBytes))
      {
        if(debug)
          println("equals!");
        
        byte[] size = new byte[2];
        arrayCopy(serialData, headerBytes.length, size, 0, 2);
        
        int numBytes = ((size[1] & 0xFF) << 8) | (size[0] & 0xFF);
        
        int totalHeaderSize = (headerBytes.length + size.length);
        int lengthToCopy = min((bytesAvailable - totalHeaderSize), numBytes);
        arrayCopy(serialData, totalHeaderSize, colors, 0, lengthToCopy);
        
        bytesMissing = numBytes - lengthToCopy;
        
        
        /*if(bytesMissing == 0)
        {
          drawFrame = true;
        }*/
        drawFrame = true;
      }
    }
    else
    {
      myPort.clear();
    }
    /*else
    {
      if(debug)
      {
        println("available: " + myPort.available() + " bytes missing: " + bytesMissing);
      }
        
      if(myPort.available() >= bytesMissing)
      {
        if(debug)
          println("read missing!");
        byte[] missingBytes = myPort.readBytes(bytesMissing);
        arrayCopy(missingBytes, 0, colors, colors.length - bytesMissing, bytesMissing);
        bytesMissing = 0;
        drawFrame = true;
      }
    }*/
  }
  
  if(drawFrame)
  {
    background(0);
    image(house, 0, 0);
    noStroke();
    //strokeWeight(1);
    
    if(debug)
      println("draw frame!");
    
    //fill((255 / file.getChildCount()) * j, 255, 255);
    int cIndex = 0;
    for (int i = 0; i < leds.size(); i++)
    {
      
      fill(UByte(colors[cIndex++]), UByte(colors[cIndex++]), UByte(colors[cIndex++]));
      PVector p = leds.get(i);
      circle(p.x, p.y, 3);
    }
    drawFrame = false;
    fx.render()
    .bloom(0.7, 20, 40)
    .compose();
  }
}



ArrayList<PVector> getEquallySpacedPoints(PVector[] vertices, float spacing) {
  ArrayList<PVector> points = new ArrayList<PVector>();
  
  // Step 1: Calculate the total length of the polyline (without closing)
  float totalLength = 0;
  float[] segmentLengths = new float[vertices.length - 1];  // One less to avoid wrap
  
  for (int i = 0; i < vertices.length - 1; i++) {  // Stop at the second-to-last vertex
    PVector v1 = vertices[i];
    PVector v2 = vertices[i + 1];
    float segmentLength = PVector.dist(v1, v2);
    segmentLengths[i] = segmentLength;
    totalLength += segmentLength;
  }
  
  // Step 2: Calculate the total number of points that can fit along the polyline
  int totalPoints = int(totalLength / spacing) +1;
  
  // Step 3: Interpolate along the shape to get the points
  float currentDistance = 0;
  int currentSegment = 0;
  
  for (int i = 0; i < totalPoints; i++) {
    // If we reach the end of the current segment, move to the next
    while (currentDistance > segmentLengths[currentSegment]) {
      currentDistance -= segmentLengths[currentSegment];
      currentSegment++;
      if (currentSegment >= segmentLengths.length) {
        return points;  // Stop if we've placed all points along the polyline
      }
    }
    
    // Interpolate along the current segment
    PVector v1 = vertices[currentSegment];
    PVector v2 = vertices[currentSegment + 1];
    float t = currentDistance / segmentLengths[currentSegment];
    
    PVector point = PVector.lerp(v1, v2, t);
    points.add(point);
    
    currentDistance += spacing;
  }
  
  
  return points;
}

PVector NormalizePoint(PVector p)
{
  float xx = ((p.x - xLimits.x) / (xLimits.y - xLimits.x));
  float yy = ((p.y - yLimits.x) / (yLimits.y - yLimits.x));
  return new PVector(xx, yy);
}

void mouseClicked() {
  //println("mouse:" + mouseX + ", " + mouseY);
  int closestIndex = ClosestLED(mouseX, mouseY);
  int cIdx = closestIndex * 3;
  println(UByte(colors[cIdx]) + ", " + UByte(colors[cIdx+1]) + ", " + UByte(colors[cIdx+2]));
}


int ClosestLED(float xx, float yy)
{
  float minDist = 999999999;
  int minIndex = -1;
  for(int i = 0; i < leds.size(); i++)
  {
    float lX = leds.get(i).x;
    float lY = leds.get(i).y;
    
    float lDist = dist(xx, yy, lX, lY);
    if(lDist < minDist)
    {
      minDist = lDist;
      minIndex = i;
    }
  }
  
  return minIndex;
}

int UByte(byte b)
{
  float fColor = Byte.toUnsignedInt(b);
  float correctedColor = pow(fColor/255.0, 1/2.2)*255.0;
  return round(correctedColor);
}
