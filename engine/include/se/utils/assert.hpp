#pragma once

#include <string>



namespace se
{
	class Assert
	{
		public:
			inline Assert(const std::string &message);
			inline const std::string &getMessage() const noexcept;


		private:
			const std::string m_message;
	};

} // namespace se


#define SE_ASSERT(cond, msg) if (cond) {} else {throw se::Assert(std::string("Condition '" #cond "' failed in file '" __FILE__ "', line ") + std::to_string(__LINE__) + ", function '" + __func__ + "' : " + msg);}



#include "assert.inl"