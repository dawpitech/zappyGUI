#include <string>
#include "IResource.hpp"

namespace GUI {
    class AResource : public IResource {
        public:
            [[nodiscard]] std::string get_type() const {
                return this->_type;
            }
            [[nodiscard]] double get_density() const {
                return this->_density;
            }
        protected:
            std::string _type;
            double _density;
    };
} // namespace GUI
