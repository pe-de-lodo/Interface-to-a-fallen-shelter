PShape file;
PShape aichi;

ArrayList<ArrayList<PVector>> ledPositions = new ArrayList<ArrayList<PVector>>();

float spacing = 10;  // Specific distance between points

void setup() {
  size(640, 360);
  // The file "bot1.svg" must be in the data folder
  // of the current sketch to load successfully
  file = loadShape("Untitled.svg");
  colorMode(HSB);
  
  println(file.getChildCount());
  
  
  
  for(int j = 0; j < file.getChildCount(); j++)
  {
    
    
    aichi = file.getChild(j);
    println(aichi.getVertexCount());
    
    PVector[] vertices = new PVector[aichi.getVertexCount()];
    ArrayList<PVector> equallySpacedPoints = new ArrayList<PVector>();
    
    for(int i = 0; i < aichi.getVertexCount(); i++)
      vertices[i] = aichi.getVertex(i);
    
    equallySpacedPoints = getEquallySpacedPoints(vertices, spacing);
    ArrayList<PVector> leds = new ArrayList<PVector>();    
    
    // Draw the shape
    beginShape();
    stroke(0);
    noFill();
    for (PVector v : vertices) {
      vertex(v.x, v.y);
    }
    endShape();
    
    // Draw the points
    fill((255 / file.getChildCount()) * j, 255, 255);
    for (PVector p : equallySpacedPoints) {
      leds.add(p);
      ellipse(p.x, p.y, 5, 5);
    }
    
    ledPositions.add(leds);
  }
  
} 

void draw(){
  
  //background(102);
  //shape(aichi, 0, 0);  
       
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
