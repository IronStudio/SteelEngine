#include <se/engine.hpp>
#include <se/assert.hpp>


int main() {
	se::Engine engine {};

	SE_ASSERT(true, "No problem");
	SE_ASSERT(false, "Failed assert");

	return 0;
}