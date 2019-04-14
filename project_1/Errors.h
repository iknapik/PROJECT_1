#ifndef ERORRS_H_
#define ERORRS_H_
#include <exception>


class InvalidStudentID :
	public std::exception
{
	
public:
	unsigned id;
	InvalidStudentID(unsigned id) : id(id) {}
	const char* what() const noexcept { return "Student ID not coressponding to any students in database!"; }
};

class InvalidProfessorID :
	public std::exception
{
public:
	unsigned id;
	InvalidProfessorID(unsigned id) : id(id) {}
	const char* what() const noexcept { return "Professor ID not coressponding to any professors in database!"; }
};

class InvalidClassID : public std::exception
{
	
public:
	unsigned id;
	InvalidClassID(unsigned id) : id(id) {}
	const char* what() const noexcept { return "Class ID not coressponding to any classes in database!: "; }
};

class InvalidID : public std::exception
{
public:
	unsigned id;
	InvalidID(unsigned id) : id(id) {}
	const char* what() const noexcept { return "ID not coressponding to any ids in database!"; }
};


#endif // !Errors_h_