#include "icg_common.h"
#include "math.h"

const static Scalar H = .7;
const static Scalar R = 2;

class BezierCurve{
private:
    class Hull{
    public:
        Hull() {
            _p1 = _p2 = _p3 = _p4 = vec3::Zero();
        }

        vec3& p1(){ return _p1; }
        vec3& p2(){ return _p2; }
        vec3& p3(){ return _p3; }
        vec3& p4(){ return _p4; }

    private:
        vec3 _p1;
        vec3 _p2;
        vec3 _p3;
        vec3 _p4;
    };
    
private:
    Hull _hull;                  ///< Bezier control points
    std::vector<vec3> _vertices; ///< Bezier evaluated points
    std::vector<Scalar> _param;  ///< Arc length parameterization
    GLuint _vao;                 ///< Vertex array objects
    GLuint _pid;          ///< GLSL program ID
    GLuint _vbo;
    
private:
    void bezier(Hull& p, int depth=0){

        Hull l;
        Hull r;
        
        //--- endpoints
        l.p1() = p.p1();
        r.p4() = p.p4();
        
        //--- one-in
        l.p2() = (p.p1() + p.p2()) / 2.0;
        r.p3() = (p.p3() + p.p4()) / 2.0;
        
        //--- interior
        l.p3() = ( l.p2() + (p.p2() + p.p3())/2.0 )/2.0;
        r.p2() = ( r.p3() + (p.p2() + p.p3())/2.0 )/2.0;
        
        //--- middle
        l.p4() = ( l.p3() + r.p2() ) / 2.0;
        r.p1() = l.p4();
        
        //--- recursion v.s. draw
        if(depth<5){
            bezier(l,depth+1);
            bezier(r,depth+1);
        } else {
            _vertices.push_back(l.p1());
        }
        _vertices.push_back(p.p4());
        
        
    }

    void compute_parameterization(){
        _param.clear();;
        if (_vertices.empty()) {
            return;
        }
        _param.resize(_vertices.size(),0);
        ///===================== TODO =====================
        ///--- TODO H5.3: Calculate curve distance from _vertices[0] to _vertice[i] and store into _param[i]
        /// Assume piece-wise linear approximation of the curve
        /// http://math.stackexchange.com/questions/12186/arc-length-of-bézier-curves
        ///================================================

        for(int i = 1 ; i<_vertices.size(); ++i){

            vec3 v = _vertices.at(i);
            vec3 v0 = _vertices.at(i-1);

            float diffX = v.x()-v0.x();
            float diffY = v.y()-v0.y();
            float diffZ = v.z()-v0.z();

            float distance = sqrt(pow(diffX, 2.0)+pow(diffY, 2.0)+pow(diffZ, 2.0));
            _param.at(i) = distance + _param.at(i-1);
        }
    }

public:
    void init(GLuint pid){
        ///--- Set the (compiled) shaders
        _pid = pid;
        
        /// Generate the vertex array
        glGenVertexArrays(ONE, &_vao);
        glBindVertexArray(_vao);
        
        /// Generate one buffer, put the resulting identifier in vertexbuffer
        glGenBuffers(ONE, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        ///--- to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);
    }
    void set_points(const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4) {
        _vertices.clear();
        _param.clear();

        ///--- initialize data
        _hull.p1() = p1;
        _hull.p2() = p2;
        _hull.p3() = p3;
        _hull.p4() = p4;

        ///--- compute bezier & parameterization
        bezier(_hull);
        compute_parameterization();
    }
    static bool cmp(const Scalar &a, const Scalar &b){
        return (a<b);
    }

    void sample_point(double t, vec3& sample) {
        if (_vertices.empty() || _param.empty()) {
            return;
        }
        ///===================== TODO =====================
        ///--- TODO H5.3: Uniformly sample a point in the curve using t in [0, 1]
        /// Assume piece-wise linear approximation
        /// The distance along the curve from _vertices[0] to sample is
        /// t * curve_length
        ///================================================

        t = sin(t);

        float curve_length = _param.at(_param.size()-1);
        float cam_dist = t*curve_length;

        //cherche les vertices
        int i = 0;
        while(i < _param.size() && _param[_param.size()-1] < cam_dist){
            i++;
        }
        vec3 v0 = _vertices[i];
        vec3 v1 = _vertices[i+1];

        float a = (cam_dist - _param[i-1])/(_param[i] - _param[i-1]);
        sample =  a*v1 + (1 - a)*v0;
    }

        
        
  
    
    void draw(const mat4& model, const mat4& view, const mat4& projection){
        if (_vertices.empty()) return;

        glUseProgram(_pid);
        glBindVertexArray(_vao);
        check_error_gl();

        ///--- Vertex Attribute ID for Vertex Positions
        GLuint position = glGetAttribLocation(_pid, "position");
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);

        ///--- vertices
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3)*_vertices.size(), &_vertices[0], GL_STATIC_DRAW);

        ///--- setup view matrices        
        GLuint projection_id = glGetUniformLocation(_pid, "projection");
        glUniformMatrix4fv(projection_id, ONE, DONT_TRANSPOSE, projection.data());
        mat4 MV = view*model;
        GLuint model_view_id = glGetUniformLocation(_pid, "model_view");
        glUniformMatrix4fv(model_view_id, ONE, DONT_TRANSPOSE, MV.data());
        check_error_gl();

        glDrawArrays(GL_LINE_STRIP, 0, _vertices.size());
        glDisableVertexAttribArray(position);
        glBindVertexArray(0);
        glUseProgram(0);
        check_error_gl();
    }
};
