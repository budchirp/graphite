#pragma once

namespace LogTypes {

enum class Error {
  UNDEFINED,
  UNKNOWN,
  INTERNAL,
  SYNTAX,
  TYPE_MISMATCH,
};

enum class Warn { SUGGESTION };

enum class Log { INFO };

} // namespace LogTypes
