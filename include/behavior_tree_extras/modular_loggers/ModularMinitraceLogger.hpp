/* Copyright [2022] <TheItalianJob>
 * Author: Gerardo Puga
 */

#ifndef BT_MODULAR_MINITRACE_LOGGER_H
#define BT_MODULAR_MINITRACE_LOGGER_H

// standard library
#include <memory>
#include <string>

// bt
#include <behaviortree_cpp_v3/loggers/bt_minitrace_logger.h>

// project
#include <behavior_tree_extras/modular_loggers/ModularAbstractLogger.hpp>

namespace behavior_tree_extras
{
/**
 * @brief Injectable MinitraceLogger logger that can be attached to a tree after creation. Allows
 *        dependency injection-ish mode of adding loggers to trees.
 */
class ModularMinitraceLogger : public ModularStatusChangeLogger
{
public:
  explicit ModularMinitraceLogger(const char* filename_json);

  /** @brief Attaches the logger to a tree and complete initialization. Needs to be called before
   *         calling any other member function. */
  void attach(BT::Tree& tree) override;

  /** @brief Flushes the logger output. Behavior undefined if the logger has not been initialized
   *         before calling.*/
  void flush() override;

private:
  std::string filename_json_;

  std::unique_ptr<BT::StatusChangeLogger> logger_;
};

}  // namespace behavior_tree_extras

#endif  // BT_MODULAR_MINITRACE_LOGGER_H
