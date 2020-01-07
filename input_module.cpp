#include "input_module.h"

void input(int &recDepth, int &pixels, vector<Shape *> &shapes, vector<string> &shapeTypes,
           vector<Point> &lights)
{
    string command;

    while((cin >> command)!=NULL)
        if(command=="recDepth")
            cin >> recDepth;
        else if(command=="pixels")
            cin >> pixels;
        else if(command=="objStart")
        {
            double ambCoeff, difCoeff, refCoeff, specCoeff, specExp;
            string type, property;

            cin >> type;
            shapeTypes.push_back(type);

            if(type=="CHECKERBOARD")
            {
                double colorOneR, colorOneG, colorOneB, colorTwoR, colorTwoG, colorTwoB;

                while(true)
                {
                    cin >> property;

                    if(property=="objEnd")
                        break;
                    else if(property=="colorOne")
                        cin >> colorOneR >> colorOneG >> colorOneB;
                    else if(property=="colorTwo")
                        cin >> colorTwoR >> colorTwoG >> colorTwoB;
                    else if(property=="ambCoeff")
                        cin >> ambCoeff;
                    else if(property=="difCoeff")
                        cin >> difCoeff;
                    else if(property=="refCoeff")
                        cin >> refCoeff;
                    else if(property=="specCoeff")
                        cin >> specCoeff;
                    else if(property=="specExp")
                        cin >> specExp;
                    else
                        cout << "Invalid property\n";
                }

                Checkerboard *checkerboard = new Checkerboard(ambCoeff, difCoeff, refCoeff,
                                                              specCoeff, specExp,
                                                              Color(colorOneR, colorOneG, colorOneB),
                                                              Color(colorTwoR, colorTwoG, colorTwoB));
                shapes.push_back(checkerboard);
            }
            else if(type=="SPHERE")
            {
                double centerX, centerY, centerZ, radius, colorR, colorG, colorB;

                while(true)
                {
                    cin >> property;

                    if(property=="objEnd")
                        break;
                    else if(property=="center")
                        cin >> centerX >> centerY >> centerZ;
                    else if(property=="radius")
                        cin >> radius;
                    else if(property=="color")
                        cin >> colorR >> colorG >> colorB;
                    else if(property=="ambCoeff")
                        cin >> ambCoeff;
                    else if(property=="difCoeff")
                        cin >> difCoeff;
                    else if(property=="refCoeff")
                        cin >> refCoeff;
                    else if(property=="specCoeff")
                        cin >> specCoeff;
                    else if(property=="specExp")
                        cin >> specExp;
                    else
                        cout << "Invalid property\n";
                }

                Sphere *sphere = new Sphere(ambCoeff, difCoeff, refCoeff, specCoeff, specExp,
                                            Point(centerX, centerY, centerZ), radius,
                                            Color(colorR, colorG, colorB));
                shapes.push_back(sphere);
            }
            else if(type=="CYLINDER")
            {
                double xCenter, zCenter, radius, yMin, yMax, colorR, colorG, colorB;

                while(true)
                {
                    cin >> property;

                    if(property=="objEnd")
                        break;
                    else if(property=="xCenter")
                        cin >> xCenter;
                    else if(property=="zCenter")
                        cin >> zCenter;
                    else if(property=="radius")
                        cin >> radius;
                    else if(property=="yMin")
                        cin >> yMin;
                    else if(property=="yMax")
                        cin >> yMax;
                    else if(property=="color")
                        cin >> colorR >> colorG >> colorB;
                    else if(property=="ambCoeff")
                        cin >> ambCoeff;
                    else if(property=="difCoeff")
                        cin >> difCoeff;
                    else if(property=="refCoeff")
                        cin >> refCoeff;
                    else if(property=="specCoeff")
                        cin >> specCoeff;
                    else if(property=="specExp")
                        cin >> specExp;
                    else
                        cout << "Invalid property\n";
                }

                Cylinder *cylinder = new Cylinder(ambCoeff, difCoeff, refCoeff, specCoeff, specExp,
                                                  xCenter, zCenter, yMin, yMax, radius,
                                                  Color(colorR, colorG, colorB));
                shapes.push_back(cylinder);
            }
            else if(type=="TRIANGLE")
            {
                double ax, ay, az, bx, by, bz, cx, cy, cz, colorR, colorG, colorB, refractiveIndex;

                while(true)
                {
                    cin >> property;

                    if(property=="objEnd")
                        break;
                    else if(property=="a")
                        cin >> ax >> ay >> az;
                    else if(property=="b")
                        cin >> bx >> by >> bz;
                    else if(property=="c")
                        cin >> cx >> cy >> cz;
                    else if(property=="color")
                        cin >> colorR >> colorG >> colorB;
                    else if(property=="ambCoeff")
                        cin >> ambCoeff;
                    else if(property=="difCoeff")
                        cin >> difCoeff;
                    else if(property=="refCoeff")
                        cin >> refCoeff;
                    else if(property=="specCoeff")
                        cin >> specCoeff;
                    else if(property=="specExp")
                        cin >> specExp;
                    else if(property=="refractiveIndex")
                        cin >> refractiveIndex;
                    else
                        cout << "Invalid property\n";
                }

                Triangle *triangle = new Triangle(ambCoeff, difCoeff, refCoeff, specCoeff, specExp,
                                                  Point(ax, ay, az), Point(bx, by, bz),
                                                  Point(cx, cy, cz), Color(colorR, colorG, colorB),
                                                  refractiveIndex);
                shapes.push_back(triangle);
            }
            else
                cout << "Invalid object type\n";
        }
        else if(command=="light")
        {
            double x, y, z;

            cin >> x >> y >> z;
            lights.push_back(Point(x, y, z));
        }
        else
            cout << "Invalid command\n";
}
