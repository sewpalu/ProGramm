#include "CYKVisualiser.hpp"

CYKVisualiser::CYKVisualiser(unsigned int wordlength)
{
  std::vector<std::vector<std::vector<CYKLink>>> outerVector;
  std::vector<std::vector<CYKLink>> innerVector;

  for (unsigned int lineCounter = 0; lineCounter < wordlength; lineCounter++)
  {
    innerVector.resize(wordlength - lineCounter);
    outerVector.push_back(innerVector);
  }

  /*std::vector<std::vector<CYKLink>> innerVector1;
  std::vector<CYKLink> innerVector2;
  CYKLink content;
  for (unsigned int i = 0; i < wordlength; i++)
  {
      innerVector2.push_back(content);
  }
  for (unsigned int i = 0; i < wordlength; i++)
  {
      innerVector1.push_back(innerVector2);
  }
  for (unsigned int i = 0; i < wordlength; i++)
  {
      outerVector.push_back(innerVector1);
  }*/
  this->matrix = outerVector;
}

CYKVisualiser::~CYKVisualiser()
{
}

void CYKVisualiser::setResult(std::pair<unsigned int, unsigned int> position,
                              std::vector<CYKLink> productions)  //
{
  /*for (unsigned int i = 0; i < productions.size(); i++)
  {
      this->matrix.at(position.first).at(position.second).push_back(productions.at(i));
  }*/
  this->matrix.at(position.first).at(position.second) = productions;
}


void CYKVisualiser::addResult(std::pair<unsigned int, unsigned int> position,
               CYKLink production)
{
  this->matrix.at(position.first).at(position.second).push_back(production);
}

void CYKVisualiser::dumpContent()
{
  std::cout << "Visualiser Content: \n";

  std::cout << "Info from dump: \n";
  for (int lineCount = static_cast<int>(this->matrix.size()) - 1; lineCount >= 0; lineCount--)
  {
    for (unsigned int colCount = 0;
         colCount < this->matrix.at(lineCount).size(); colCount++)
    {
      std::string displayPlace = "";
      std::cout << "Line Count: " << lineCount << " | Column Count: " <<
      colCount << "\n";
      for (unsigned int elementCount = 0;
           elementCount < this->matrix.at(lineCount).at(colCount).size();
           elementCount++)
      {
        try
        {
          if (!(displayPlace.find(this->matrix.at(lineCount)
                                      .at(colCount)
                                      .at(elementCount)
                                      .getRoot()
                                      .getIdentifier()) != std::string::npos))
          {
            if (displayPlace.size() > 0)
            {
              displayPlace = displayPlace + ", ";
            }
            displayPlace = displayPlace + this->matrix.at(lineCount)
                                              .at(colCount)
                                              .at(elementCount)
                                              .getRoot()
                                              .getIdentifier();
          }
        }
        catch (...)
        {
          std::cout << "Error when writing!\n";
          std::cout << "Matrix size i (lines): " << this->matrix.size() << "\n";
          std::cout << "Matrix size j (colunns): "
                    << this->matrix.at(lineCount).size() << "\n";
          std::cout << "Matrix size k (CYKLinks in Cell): "
                    << this->matrix.at(lineCount).at(colCount).size() << "\n";
          std::cout << "lineCount: " << lineCount << "colCount: " << colCount
                    << "elementCount: " << elementCount << "\n";
        }
      }
      std::cout << displayPlace;
      std::cout << " | ";
    }
    std::cout << "\n";
  }
  std::cout << "-----------\n";
}

void CYKVisualiser::dumpAll()
{
  std::cout << "Dump All"
            << "\n";
  for (int i = static_cast<int>(this->matrix.size()) - 1; i >= 0; i--)
  {
    for (unsigned int j = 0; j < this->matrix.at(i).size(); j++)
    {
      std::string displayPlace =
          "Symbols:" + std::to_string(this->matrix.at(i).at(j).size()) + "-";
      for (unsigned int k = 0; k < this->matrix.at(i).at(j).size(); k++)
      {
        try
        {
          // if
          // (!(displayPlace.find(this->matrix.at(i).at(j).at(k).getRoot().getIdentifier())
          // != std::string::npos)) {
          if (displayPlace.size() > 0)
          {
            displayPlace = displayPlace + ", ";
          }
          displayPlace =
              displayPlace +
              this->matrix.at(i).at(j).at(k).getRoot().getIdentifier()/* +
              std::to_string(
                  this->matrix.at(i).at(j).at(k).getProductions().size())*/;
          if (this->matrix.at(i).at(j).at(k).getRoot().getIdentifier() == "")
          {
            displayPlace = displayPlace + "EmptyNonterminal";
          }
          //}
        }
        catch (...)
        {
          std::cout << "Error when writing!\n";
          std::cout << "Matrix size i: " << this->matrix.size() << "\n";
          std::cout << "Matrix size j: " << this->matrix.at(i).size() << "\n";
          std::cout << "Matrix size k: " << this->matrix.at(i).at(j).size()
                    << "\n";
          std::cout << "i: " << i << "j: " << j << "k: " << k << "\n";
        }
      }
      std::cout << displayPlace;
      std::cout << " | ";
    }
    std::cout << "\n";
  }
  std::cout << "-----------\n";
}

void CYKVisualiser::dumpContent(unsigned int coordinate1,
                                unsigned int coordinate2,
                                std::string nonterminalIdentifier)
{
  // std::cout << "Visualiser Content: \n";
  std::cout << "Highlighting: (" << coordinate1 << "/" << coordinate2
            << ") for Nonterminal " << nonterminalIdentifier << "\n";

  std::pair<unsigned int, unsigned int> dependency1Coordinates;
  std::pair<unsigned int, unsigned int> dependency2Coordinates;

  if (coordinate1 >= this->matrix.size())
  {
    std::cout << "Called dump content with out of range coordinates!\n";
    return;
  }

  if (coordinate2 >= this->matrix.at(coordinate1).size())
  {
    std::cout << "Called dump content with out of range coordinates!\n";
    return;
  }

  for (unsigned int i = 0;
       i < this->matrix.at(coordinate1).at(coordinate2).size(); i++)
  {
    std::cout << "Checking if with i = " << i << "\n";
    if (this->matrix.at(coordinate1)
            .at(coordinate2)
            .at(i)
            .getRoot()
            .getIdentifier() == nonterminalIdentifier)
    {
      std::cout << "Successful if for: " << nonterminalIdentifier << "\n";
      dependency1Coordinates.first = this->matrix.at(coordinate1)
                                         .at(coordinate2)
                                         .at(i)
                                         .getProductions()
                                         .at(0)
                                         .first.first;
      dependency1Coordinates.second = this->matrix.at(coordinate1)
                                          .at(coordinate2)
                                          .at(i)
                                          .getProductions()
                                          .at(0)
                                          .first.second;
      dependency2Coordinates.first = this->matrix.at(coordinate1)
                                         .at(coordinate2)
                                         .at(i)
                                         .getProductions()
                                         .at(1)
                                         .first.first;
      dependency2Coordinates.second = this->matrix.at(coordinate1)
                                          .at(coordinate2)
                                          .at(i)
                                          .getProductions()
                                          .at(1)
                                          .first.second;

      break;
    }
  }

  std::cout << "Visualiser Content: \n";

  for (int i = static_cast<int>(this->matrix.size()) - 1; i >= 0; i--)
  {
    for (int j = 0; j < this->matrix.at(i).size() - i; j++)
    {
      if (dependency1Coordinates.first == i)
      {
        if (dependency1Coordinates.second == j)
        {
          std::cout << "P1: ";
        }
      }

      if (dependency2Coordinates.first == i)
      {
        if (dependency2Coordinates.second == j)
        {
          std::cout << "P2: ";
        }
      }
      std::cout << "Creating display place\n";
      std::string displayPlace = "";
      for (unsigned int k = 0; k < this->matrix.at(i).at(j).size(); k++)
      {
        try
        {
          if (!(displayPlace.find(
                    this->matrix.at(i).at(j).at(k).getRoot().getIdentifier()) !=
                std::string::npos))
          {
            if (displayPlace.size() > 0)
            {
              displayPlace = displayPlace + ", ";
            }
            displayPlace =
                displayPlace +
                this->matrix.at(i).at(j).at(k).getRoot().getIdentifier();
          }
        }
        catch (...)
        {
          std::cout << "Error when writing!\n";
          std::cout << "Matrix size i: " << this->matrix.size() << "\n";
          std::cout << "Matrix size j: " << this->matrix.at(i).size() << "\n";
          std::cout << "Matrix size k: " << this->matrix.at(i).at(j).size()
                    << "\n";
          std::cout << "i: " << i << "j: " << j << "k: " << k << "\n";
        }
      }
      std::cout << displayPlace;
      std::cout << " | ";
    }
    std::cout << "\n";
  }
  std::cout << "-----------\n";
}

// Option to handle all recursive calls to further functions in order to return
// all possible SyntaxTrees that produce the word within the given grammar
std::vector<SyntaxTree> CYKVisualiser::convertToSyntaxTrees(
    FormalGrammar grammar)
{
  //std::cout << "Converting to SyntaxTree: \n";
  // Important to keep track of how often the functions need to be called
  std::size_t wordLength = this->matrix.size();

  // Vector of all starting positions in the first field of the CYKMatrix
  std::vector<unsigned int> startPositions;

  // Loop through all Nonterminals in the field of the CYKMatrix most upwards
  // and leftwards
  for (unsigned int i = 0; i < this->matrix.at(wordLength - 1).at(0).size();
       i++)
  {
    // Write all starting positions to vector
    if (this->matrix.at(wordLength - 1).at(0).at(i).getRoot().getIdentifier() ==
        grammar.start.getIdentifier())
      startPositions.push_back(i);
  }

  // Vector for resulting trees
  std::vector<SyntaxTree> resultTrees;

  // For development: Show all identifiers in CYKMatrix
  //this->dumpAll();

  // Create the SyntaxTree for each possible starting position
  for (unsigned int startPosCounter = 0;
       startPosCounter < startPositions.size(); startPosCounter++)
  {
    /*std::cout
        << "Hello from the loop ----------------------------- (iteration: "
        << startPosCounter << ") \n";*/
    CYKLink startPosition = this->matrix.at(wordLength - 1)
                                .at(0)
                                .at(startPositions.at(startPosCounter));
    /*std::cout << "Start position: " << startPosition.getRoot().getIdentifier()
              << "\n";*/
    STNode treeRoot(
        std::make_unique<Nonterminal>(startPosition.getRoot().getIdentifier()));

    treeRoot.addChildren(this->addChildrenFromLink(startPosition));

    SyntaxTree possibleTree(treeRoot);
    resultTrees.push_back(possibleTree);
  }

  return resultTrees;
}

void CYKVisualiser::saveStep()
{
  steps.push_back(matrix);
}

std::vector<STNode> CYKVisualiser::addChildrenFromLink(CYKLink inputLink)
{
  std::vector<STNode> children;

  // std::cout << "Kinder gefunden: " << inputLink.getProductions().size() <<
  // "\n";
  /*std::cout << "Child counter: " << inputLink.getProductions().size() << "\n";
  std::cout << "children: \n";
  for (unsigned int i = 0; i < inputLink.getProductions().size(); i++)
  {
    std::cout
        << inputLink.getProductions().at(i).second.getRoot().getIdentifier()
        << "   ";
  }
  std::cout << "\n";*/
  for (unsigned int childCounter = 0;
       childCounter < inputLink.getProductions().size(); childCounter++)
  {
    // std::cout << "Funktionsaufruf aus Schleife!\n";
    STNode child(std::make_unique<Nonterminal>(inputLink.getProductions()
                     .at(childCounter)
                     .second.getRoot()
                     .getIdentifier()));
    child.addChildren(addChildrenFromLink(
        inputLink.getProductions().at(childCounter).second));
    // STNode(Symbol value_input, std::vector<STNode> children);
    // std::cout << "Kind: " << child.value.getIdentifier() << "\n";
    children.push_back(child);
  }

  /*if (children.size() > 2)
  {
      std::cout << "Converter Error";
  }*/

  // if (children.size() == 0) std::cout << "Letzter Wert: " <<
  // inputLink.getRoot().getIdentifier() << "\n\n";

  return children;
}
