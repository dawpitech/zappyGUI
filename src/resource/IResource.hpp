#include <string>
namespace GUI {
    class IResource {
        public:
            [[nodiscard]] virtual std::string get_type() const = 0;
            [[nodiscard]] virtual std::string get_density() const = 0;
    };
} // namespace GUI
