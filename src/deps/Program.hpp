/**
* @file Program.hpp
*
* @author Adam Dziedzic (dziedada@umich.edu), Romario Pashollari (rpash@umich.edu)
* @date 2017-08-22
*/
#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <epoxy/gl.h>

class Program
{
public:
	/**
	* @brief Set up a shader program
	*/
	Program();

	/**
	* @brief Cleans up this shader program
	*/
	~Program();

	// Should not move or copy construct this object
	Program(Program&&) = delete;
	Program& operator=(Program&) = delete;
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;

	// Attribute index getters
	GLint vert() const noexcept { return vert_; }

	GLint color() const noexcept { return color_; }

	GLuint prog() const noexcept { return prog_; }

private:
	// The program identifier
	GLuint prog_;

	// Locations of attributes
	GLint vert_, color_;
};

#endif
