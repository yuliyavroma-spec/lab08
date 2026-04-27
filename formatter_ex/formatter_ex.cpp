#include "formatter_ex.h"
#include "../formatter_lib/formatter.h"

std::string formatter_ex(const std::string& message) {
    return ">>> " + formatter(message) + " <<<";
}
