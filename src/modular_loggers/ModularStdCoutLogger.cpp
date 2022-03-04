/* Copyright [2022] <TheItalianJob>
 * Author: Gerardo Puga
 */

// standard library
#include <memory>

// project
#include <behavior_tree_extras/modular_loggers/ModularStdCoutLogger.hpp>

namespace behavior_tree_extras
{
void ModularStdCoutLogger::flush()
{
  logger_->flush();
}

void ModularStdCoutLogger::attach(BT::Tree& tree)
{
  logger_ = std::make_unique<BT::StdCoutLogger>(tree);
}

}  // namespace behavior_tree_extras
