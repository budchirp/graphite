#pragma once

namespace LogTypes {

enum class Error {
  SYNTAX,
  TYPE_MISMATCH,
};

enum class Warn { SUGGESTION };

enum class Log { INFO };

} // namespace LogTypes
