#ifndef ERORRS_H_
#define ERORRS_H_
#include <exception>

namespace project
{
	enum class ErrorCode
	{
		INVALID_ID, INVALID_STUDENT_ID, INVALID_PROFESSOR_ID, INVALID_CLASS_ID,
		INVALID_MARK_ID, INVALID_FIRSTNAME, INVALID_SURNAME, INVALID_PESEL, INVALID_ADRESS,
		INVALID_CITY, INVALID_SUBJECT_NAME, CLASS_IN_USE, DISSALOWED_CHARACTER
	};

	class DatabaseError :
		public std::exception
	{
		ErrorCode m_code;
		unsigned m_id;
	public:
		
		DatabaseError(ErrorCode code, unsigned id = 0) : m_code(code), m_id(id) {}
		const char* what() const noexcept
		{
			switch (m_code)
			{
			case ErrorCode::INVALID_ID:
				return "Invalid ID!";
			case ErrorCode::INVALID_STUDENT_ID:
				return "Invalid student ID!";
			case ErrorCode::INVALID_ADRESS:
				return "Invalid adress!";
			case ErrorCode::INVALID_CLASS_ID:
				return "Invalid class ID!";
			case ErrorCode::INVALID_PROFESSOR_ID:
				return "Invalid professor ID!";
			case ErrorCode::INVALID_MARK_ID:
				return "Invalid mark ID!";
			case ErrorCode::INVALID_FIRSTNAME:
				return "Invalid firstname!";
			case ErrorCode::INVALID_SURNAME:
				return "Invalid surname!";
			case ErrorCode::INVALID_PESEL:
				return "Invalid PESEL number!";
			case ErrorCode::INVALID_SUBJECT_NAME:
				return "Invalid name of the subject!";
			case ErrorCode::INVALID_CITY:
				return "Invalid name of the city!";
			case ErrorCode::CLASS_IN_USE:
				return "Cannot remove class, there are still students in it!";
			case ErrorCode::DISSALOWED_CHARACTER:
				return "Dissalowed character used!";
			}
		}
		const ErrorCode error_code() const noexcept { return m_code; }
		const unsigned id() const noexcept { return m_id; }
	};

}

#endif // !Errors_h_