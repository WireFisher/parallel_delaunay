#ifndef H_DELAUNAY_2D
#define H_DELAUNAY_2D

#include <vector>
#include <iostream>
//#include <unistd.h>


template <typename T>
class Triangle;

template <typename T>
class Point
{
    public:
        Point(): x(0), y(0) {};
        Point(T _x, T _y): x(_x), y(_y) {};
        Point(const Point &v): x(v.x), y(v.y) {};
        float dist(const Point &v) { return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y)); };

        T x;
        T y;
        std::vector<Triangle<T>* > triangle;
};


template <typename T>
class Edge
{
    public:
        Edge(Point<T> *p1, Point<T> *p2) : p1(p1), p2(p2) {};
        Edge(const Edge e) : p1(e.p1), p2(e.p2) {};

        Point<T> *p1;
        Point<T> *p2;
};


template <typename T>
class Triangle
{
    public:
        Triangle(const Point<T> *_p1, const Point<T> *_p2, const Point<T> *_p3): p1(_p1), p2(_p2), p3(_p3)
        {
            e1 = new Edge<T>(_p1, _p2);
            e2 = new Edge<T>(_p2, _p3);
            e3 = new Edge<T>(_p3, _p1);
        };

        ~Triangle()
        {
            delete e1;
            delete e2;
            delete e3;
        }
    
        bool containsVertex(const Point<T> &v) { return *p1 == v || *p2 == v || *p3 == v; };
        bool circumCircleContains(const Point<T> &);
    
        Point<T> *p1;
        Point<T> *p2;
        Point<T> *p3;
        Edge<T> *e1;             
        Edge<T> *e2;
        Edge<T> *e3;
};


template <typename T>
bool Triangle<T>::circumCircleContains(const Point<T> &v)
{
    float ab = (p1.x * p1.x) + (p1.y * p1.y);
    float cd = (p2.x * p2.x) + (p2.y * p2.y);
    float ef = (p3.x * p3.x) + (p3.y * p3.y);

    float circum_x = (ab * (p3.y - p2.y) + cd * (p1.y - p3.y) + ef * (p2.y - p1.y)) / (p1.x * (p3.y - p2.y) + p2.x * (p1.y - p3.y) + p3.x * (p2.y - p1.y)) / 2.f;
    float circum_y = (ab * (p3.x - p2.x) + cd * (p1.x - p3.x) + ef * (p2.x - p1.x)) / (p1.y * (p3.x - p2.x) + p2.y * (p1.x - p3.x) + p3.y * (p2.x - p1.x)) / 2.f;
    float circum_radius = sqrtf(((p1.x - circum_x) * (p1.x - circum_x)) + ((p1.y - circum_y) * (p1.y - circum_y)));

    float dist = sqrtf(((v.x - circum_x) * (v.x - circum_x)) + ((v.y - circum_y) * (v.y - circum_y)));
    return dist <= circum_radius;
};


template <typename T>
class Delaunay
{
    public:
        const std::vector<Triangle<T> >& triangulate(std::vector<Point<T> > &);
        void create_relationships_from_vertices_to_triangles();

        const std::vector<Triangle<T> >& getTriangles() const { return _triangles; };
        const std::vector<Edge<T> >& getEdges() const { return _edges; };
        const std::vector<Point<T> >& getVertices() const { return _vertices; };

    private:
        std::vector<Triangle<T> > _triangles;
        std::vector<Edge<T> > _edges;
        std::vector<Point<T> > _vertices;
};


template <typename T>
void Delaunay<T>::create_relationships_from_vertices_to_triangles()
{
    for(unsigned int i = 0; i < _triangles.size(); i++){
    }
}


template <typename T>
const std::vector<Triangle<T> >& Delaunay<T>::triangulate(std::vector<Point<T> > &vertices)
{
    // Store the vertices localy
    _vertices = vertices;

    // Determinate the super triangle
    float minX = vertices[0].x;
    float minY = vertices[0].y;
    float maxX = minX;
    float maxY = minY;

    for(std::size_t i = 0; i < vertices.size(); ++i) 
    {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
    }
    
    float dx = maxX - minX;
    float dy = maxY - minY;
    float deltaMax = std::max(dx, dy);
    float midx = (minX + maxX) / 2.f;
    float midy = (minY + maxY) / 2.f;

    Point<T> p1(midx - 20 * deltaMax, midy - deltaMax);
    Point<T> p2(midx, midy + 20 * deltaMax);
    Point<T> p3(midx + 20 * deltaMax, midy - deltaMax);   

    //std::cout << "Super triangle " << std::endl << Triangle(p1, p2, p3) << std::endl;
    
    // Create a list of triangles, and add the supertriangle in it
    _triangles.push_back(Triangle<T>(p1, p2, p3));

    for(typename std::vector<Point<T> >::iterator p = vertices.begin(); p != vertices.end(); p++)
    {
        //std::cout << "Traitement du point " << *p << std::endl;
        //std::cout << "_triangles contains " << _triangles.size() << " elements" << std::endl; 

        std::vector<Triangle<T> > badTriangles;
        std::vector<Edge<T> > polygon;

        for(typename std::vector<Triangle<T> >::iterator t = _triangles.begin(); t != _triangles.end(); t++)
        {
            //std::cout << "Processing " << std::endl << *t << std::endl;

            if(t->circumCircleContains(*p))
            {
                //std::cout << "Pushing bad triangle " << *t << std::endl;
                badTriangles.push_back(*t);
                polygon.push_back(t->e1);   
                polygon.push_back(t->e2);   
                polygon.push_back(t->e3);   
            }
            else
            {
                //std::cout << " does not contains " << *p << " in his circum center" << std::endl;
            }
        }

        for(typename std::vector<Triangle<T> >::iterator bt = badTriangles.begin(); bt != badTriangles.end(); bt++)
            for(typename std::vector<Triangle<T> >::iterator t = _triangles.begin(); t != _triangles.end(); t++)
                if(*bt == *t) {
                    _triangles.erase(t);
                    break;
                }
        
    /*    
        _triangles.erase(std::remove_if(_triangles.begin(), _triangles.end(), [badTriangles](Triangle<T> &t){
            for(std::vector<Triangle<T> >::iterator bt = badTriangles.begin(); bt != badTriangles.end(); bt++)
            {   
                if(*bt == t)
                {
                    //std::cout << "Removing bad triangle " << std::endl << *bt << " from _triangles" << std::endl;
                    return true;        
                }
            }
            return false;
        }), _triangles.end());
    */
        std::vector<Edge<T> > badEdges;
        //printf("badEdges: %u, polygon: %u.\n", badEdges.size(), polygon.size());
        for(typename std::vector<Edge<T> >::iterator e1 = polygon.begin(); e1 != polygon.end(); e1++)
        {
            for(typename std::vector<Edge<T> >::iterator e2 = polygon.begin(); e2 != polygon.end(); e2++)
            {
                //printf("e1: %p, e2: %p. begin: %p, end: %p\n", e1, e2, polygon.begin(), polygon.end());
                //sleep(0.1);
                if(e1 == e2)
                    continue;
                
                if(*e1 == *e2)
                {
                    badEdges.push_back(*e1);    
                    badEdges.push_back(*e2);    
                }
            }
        }

        for(typename std::vector<Edge<T> >::iterator it = badEdges.begin(); it != badEdges.end(); it++)
            for(typename std::vector<Edge<T> >::iterator e = polygon.begin(); e != polygon.end(); e++)
                if(*it == *e) {
                    polygon.erase(e);
                    break;
                }
        /*
        polygon.erase(std::remove_if(polygon.begin(), polygon.end(), [badEdges](Edge<T> &e){
            for(std::vector<Edge<T> >::iterator it = badEdges.begin(); it != badEdges.end(); it++)
            {
                if(*it == e)
                    return true;
            }
            return false;
        }), polygon.end());
        */
        for(typename std::vector<Edge<T> >::iterator e = polygon.begin(); e != polygon.end(); e++)
            _triangles.push_back(Triangle<T>(e->p1, e->p2, *p));
    
    }

    for(typename std::vector<Triangle<T> >::iterator t = _triangles.begin(); t != _triangles.end();)
        if(t->containsVertex(p1) || t->containsVertex(p2) || t->containsVertex(p3))
            _triangles.erase(t);
        else
            t++;
    /*
    _triangles.erase(std::remove_if(_triangles.begin(), _triangles.end(), [p1, p2, p3](Triangle<T> &t){
        return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
    }), _triangles.end());
    */
    for(typename std::vector<Triangle<T> >::iterator t = _triangles.begin(); t != _triangles.end(); t++)
    {
        _edges.push_back(t->e1);
        _edges.push_back(t->e2);
        _edges.push_back(t->e3);
    } 

    return _triangles;
};


template <typename T>
std::ostream & operator << (std::ostream &str, Point<T> const &point) 
{
    return str << "Point x: " << point.x << " y: " << point.y;
}

template <typename T>
bool operator == (Point<T> v1, Point<T> v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y);
}


template <class T>
inline std::ostream &operator << (std::ostream &str, Edge<T> const &e)
{
    return str << "Edge " << *e.p1 << ", " << *e.p2;
}

template <class T>
inline bool operator == (const Edge<T> & e1, const Edge<T> & e2)
{
    return  (*e1.p1 == *e2.p1 && *e1.p2 == *e2.p2) ||
            (*e1.p1 == *e2.p2 && *e1.p2 == *e2.p1);
}


template <class T>
inline std::ostream &operator << (std::ostream &str, const Triangle<T> & t)
{
    return str << "Triangle:" << std::endl << "\t" << t.p1 << std::endl << "\t" << t.p2 << std::endl << "\t" << t.p3 << std::endl << "\t" << t.e1 << std::endl << "\t" << t.e2 << std::endl << "\t" << t.e3 << std::endl;
        
}

template <class T>
inline bool operator == (const Triangle<T> &t1, const Triangle<T> &t2)
{
    return  (t1.p1 == t2.p1 || t1.p1 == t2.p2 || t1.p1 == t2.p3) &&
            (t1.p2 == t2.p1 || t1.p2 == t2.p2 || t1.p2 == t2.p3) && 
            (t1.p3 == t2.p1 || t1.p3 == t2.p2 || t1.p3 == t2.p3);
}


#endif
