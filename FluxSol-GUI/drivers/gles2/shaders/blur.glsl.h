/* WARNING, THIS FILE WAS GENERATED, DO NOT EDIT */
#ifndef BLUR_GLSL_HGLES2_120
#define BLUR_GLSL_HGLES2_120


#include "drivers/gles2/shader_gles2.h"


class BlurShaderGLES2 : public ShaderGLES2 {

	 virtual String get_shader_name() const { return "BlurShaderGLES2"; }
public:

	enum Conditionals {
		USE_GLES_OVER_GL,
	};

	enum Uniforms {
		TEXTURE,
	};

	_FORCE_INLINE_ int get_uniform(Uniforms p_uniform) const { return _get_uniform(p_uniform); }

	_FORCE_INLINE_ void set_conditional(Conditionals p_conditional,bool p_enable)  {  _set_conditional(p_conditional,p_enable); }

	#define _FU if (get_uniform(p_uniform)<0) return; ERR_FAIL_COND( get_active()!=this );

 	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, float p_value) { _FU glUniform1f(get_uniform(p_uniform),p_value); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, double p_value) { _FU glUniform1f(get_uniform(p_uniform),p_value); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, uint8_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, int8_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, uint16_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, int16_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, uint32_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, int32_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Color& p_color) { _FU GLfloat col[4]={p_color.r,p_color.g,p_color.b,p_color.a}; glUniform4fv(get_uniform(p_uniform),1,col); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Vector2& p_vec2) { _FU GLfloat vec2[2]={p_vec2.x,p_vec2.y}; glUniform2fv(get_uniform(p_uniform),1,vec2); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Vector3& p_vec3) { _FU GLfloat vec3[3]={p_vec3.x,p_vec3.y,p_vec3.z}; glUniform3fv(get_uniform(p_uniform),1,vec3); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, float p_a, float p_b) { _FU glUniform2f(get_uniform(p_uniform),p_a,p_b); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, float p_a, float p_b, float p_c) { _FU glUniform3f(get_uniform(p_uniform),p_a,p_b,p_c); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, float p_a, float p_b, float p_c, float p_d) { _FU glUniform4f(get_uniform(p_uniform),p_a,p_b,p_c,p_d); }

	_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Transform& p_transform) {  _FU

		const Transform &tr = p_transform;

		GLfloat matrix[16]={ /* build a 16x16 matrix */
			tr.basis.elements[0][0],
			tr.basis.elements[1][0],
			tr.basis.elements[2][0],
			0,
			tr.basis.elements[0][1],
			tr.basis.elements[1][1],
			tr.basis.elements[2][1],
			0,
			tr.basis.elements[0][2],
			tr.basis.elements[1][2],
			tr.basis.elements[2][2],
			0,
			tr.origin.x,
			tr.origin.y,
			tr.origin.z,
			1
		};


	glUniformMatrix4fv(get_uniform(p_uniform),1,false,matrix);


	}

		_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Matrix32& p_transform) {  _FU

		const Matrix32 &tr = p_transform;

		GLfloat matrix[16]={ /* build a 16x16 matrix */
			tr.elements[0][0],
			tr.elements[0][1],
			0,
			0,
			tr.elements[1][0],
			tr.elements[1][1],
			0,
			0,
			0,
			0,
			1,
			0,
			tr.elements[2][0],
			tr.elements[2][1],
			0,
			1
		};


	glUniformMatrix4fv(get_uniform(p_uniform),1,false,matrix);


	}

		_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const CameraMatrix& p_matrix) {  _FU

		GLfloat matrix[16];

		for (int i=0;i<4;i++) {
			for (int j=0;j<4;j++) {

				matrix[i*4+j]=p_matrix.matrix[i][j];
			}
		}

		glUniformMatrix4fv(get_uniform(p_uniform),1,false,matrix);
	}; 

#undef _FU


	virtual void init() {

		static const Enum *_enums=NULL;
		static const EnumValue *_enum_values=NULL;
		static const char* _conditional_strings[]={
			"#define USE_GLES_OVER_GL\n",
		};

		static const char* _uniform_strings[]={
			"texture",
		};

		static AttributePair _attribute_pairs[]={
			{"vertex_attrib",0},
			{"uv_in",4},
		};

		static TexUnitPair *_texunit_pairs=NULL;
		static const char _vertex_code[]={
10,35,105,102,100,101,102,32,85,83,69,95,71,76,69,83,95,79,86,69,82,95,71,76,10,35,100,101,102,105,110,101,32,109,101,100,105,117,109,112,10,35,100,101,102,105,110,101,32,104,105,103,104,112,10,35,101,108,115,101,10,112,114,101,99,105,115,105,111,110,32,109,101,100,105,117,109,112,32,102,108,111,97,116,59,10,112,114,101,99,105,115,105,111,110,32,109,101,100,105,117,109,112,32,105,110,116,59,10,35,101,110,100,105,102,10,10,97,116,116,114,105,98,117,116,101,32,104,105,103,104,112,32,118,101,99,52,32,118,101,114,116,101,120,95,97,116,116,114,105,98,59,32,47,47,32,97,116,116,114,105,98,58,48,10,97,116,116,114,105,98,117,116,101,32,118,101,99,50,32,117,118,95,105,110,59,32,47,47,32,97,116,116,114,105,98,58,52,10,10,118,97,114,121,105,110,103,32,118,101,99,50,32,117,118,95,111,117,116,59,10,10,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,10,32,32,32,32,32,32,32,32,99,111,108,111,114,95,105,110,116,101,114,112,32,61,32,99,111,108,111,114,95,97,116,116,114,105,98,59,10,32,32,32,32,32,32,32,32,117,118,95,105,110,116,101,114,112,32,61,32,117,118,95,97,116,116,114,105,98,59,10,32,32,32,32,32,32,32,32,118,101,99,52,32,111,117,116,118,101,99,32,61,32,118,101,99,52,40,118,101,114,116,101,120,44,32,49,46,48,41,59,10,32,32,32,32,32,32,32,32,111,117,116,118,101,99,32,61,32,101,120,116,114,97,95,109,97,116,114,105,120,32,42,32,111,117,116,118,101,99,59,10,32,32,32,32,32,32,32,32,111,117,116,118,101,99,32,61,32,109,111,100,101,108,118,105,101,119,95,109,97,116,114,105,120,32,42,32,111,117,116,118,101,99,59,10,32,32,32,32,32,32,32,32,103,108,95,80,111,115,105,116,105,111,110,32,61,32,112,114,111,106,101,99,116,105,111,110,95,109,97,116,114,105,120,32,42,32,111,117,116,118,101,99,59,10,125,10,10,		0};

		static const int _vertex_code_start=1;
		static const char _fragment_code[]={
10,35,105,102,100,101,102,32,85,83,69,95,71,76,69,83,95,79,86,69,82,95,71,76,10,35,100,101,102,105,110,101,32,109,101,100,105,117,109,112,10,35,100,101,102,105,110,101,32,104,105,103,104,112,10,35,101,108,115,101,10,112,114,101,99,105,115,105,111,110,32,109,101,100,105,117,109,112,32,102,108,111,97,116,59,10,112,114,101,99,105,115,105,111,110,32,109,101,100,105,117,109,112,32,105,110,116,59,10,35,101,110,100,105,102,10,10,32,47,47,32,116,101,120,117,110,105,116,58,48,10,117,110,105,102,111,114,109,32,115,97,109,112,108,101,114,50,68,32,116,101,120,116,117,114,101,59,10,118,97,114,121,105,110,103,32,118,101,99,50,32,117,118,95,111,117,116,59,10,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,10,32,32,32,32,32,32,32,32,118,101,99,52,32,99,111,108,111,114,32,61,32,99,111,108,111,114,95,105,110,116,101,114,112,59,10,10,32,32,32,32,32,32,32,32,99,111,108,111,114,32,42,61,32,116,101,120,116,117,114,101,50,68,40,32,116,101,120,116,117,114,101,44,32,32,117,118,95,105,110,116,101,114,112,32,41,59,10,10,32,32,32,32,32,32,32,32,103,108,95,70,114,97,103,67,111,108,111,114,32,61,32,99,111,108,111,114,59,10,125,10,10,		0};

		static const int _fragment_code_start=28;
		setup(_conditional_strings,1,_uniform_strings,1,_attribute_pairs,2, _texunit_pairs,0,_vertex_code,_fragment_code,_vertex_code_start,_fragment_code_start);
	};

};

#endif

