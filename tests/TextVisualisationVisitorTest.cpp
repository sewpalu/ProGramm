#include <memory>

#include "gtest/gtest.h"

#include "Nonterminal.hpp"
#include "STNode.hpp"
#include "STVisualiser.hpp"
#include "Terminal.hpp"
#include "TextVisualisationVisitor.hpp"

class TextVisualisationVisitorTest : public testing::Test
{
protected:
  TextVisualisationVisitorTest() = default;
  virtual ~TextVisualisationVisitorTest() = default;

  virtual void SetUp() override
  {
  }

  virtual void TearDown() override
  {
  }

protected:
  TextVisualisationVisitor m_testobject{};
};

TEST_F(TextVisualisationVisitorTest, CYKTest)
{
  // TODO
}

TEST_F(TextVisualisationVisitorTest, SyntaxTreeTest)
{
  {
    const auto syntax_tree = std::make_shared<const STNode>(
        std::make_unique<Nonterminal>("Root"),
        std::vector<STNode>{
            STNode{std::make_unique<Terminal>("Child1", "1")},
            STNode{std::make_unique<Nonterminal>("Subtree"),
                   {
                       STNode{std::make_unique<Terminal>("Child2", "2")},
                       STNode{std::make_unique<Terminal>("Child3", "3")},
                   }},
            STNode{std::make_unique<Terminal>("Child4", "4")},
        });
    const auto st_visualiser = STVisualiser{syntax_tree};
    m_testobject.visitSTVisualiser(st_visualiser);

    auto result = m_testobject.toString();
    EXPECT_EQ(result, "Root\n├──Child1(1)\n├──Subtree\n│  ├──Child2(2)\n│  "
                      "└──Child3(3)\n└──Child4(4)\n")
        << "Normal doubly nested syntax tree isn't visualised properly.";
  }

  {
    const auto syntax_tree =
        std::make_shared<const STNode>(std::make_unique<Terminal>("Root", "0"));
    const auto st_visualiser = STVisualiser{syntax_tree};
    m_testobject.visitSTVisualiser(st_visualiser);

    auto result = m_testobject.toString();
    EXPECT_EQ(result, "Root(0)\n")
        << "Empty syntax tree isn't visualised properly.";
  }
}
