/* Copyright [2022] <TheItalianJob>
 * Author: Gerardo Puga
 */

#pragma once

// standard library
#include <memory>

// project
#include <behavior_tree_extras/BTExecutionResult.hpp>

namespace behavior_tree_extras
{
class BehaviorTreeManagerInterface
{
public:
  using Ptr = std::unique_ptr<BehaviorTreeManagerInterface>;

  virtual ~BehaviorTreeManagerInterface() = default;

  virtual BTExecutionResult run() = 0;

  virtual void halt() = 0;
};

}  // namespace behavior_tree_extras
