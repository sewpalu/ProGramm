#!/bin/sh

##
## Execute in project root like this:
## $> tests/integration_test.sh
##

##
## Tests for the expected output given a grammar, word and visualisation
##
## \param $1  Grammar (JSON string, grammar must be named "default" according
##            to the program documentation)
## \param $2  Input word
## \param $3  Visualisation ("cyk" or "st")
## \param $4  Expected output
##
test_case () {
  grammar_file=$(mktemp)
  output_file=$(mktemp)
  error_file=$(mktemp)
  echo "$1" > $grammar_file
  if ./build/pro_gramm parse "$2" -g $grammar_file "$3" 1>$output_file 2>$error_file
  then
    if grep -F "$(echo -e $4)" $output_file &>/dev/null
    then
      tput setaf 2
      echo "Test successful"
      tput sgr0
    else
      tput setaf 1
      echo "Test unsuccessful"
      echo
      echo "Expected:"
      tput sgr0

      echo -e "$4"
      echo

      tput setaf 1
      echo
      echo "Actual:"
      tput sgr0

      cat $output_file
      echo
    fi
  else
    exit_code=$?

    tput setaf 1
    echo "Programm unsuccessful (Exit code $exit_code?)"
    echo
    echo "stdout:"
    tput sgr0

    cat $output_file
    echo

    tput setaf 1
    echo
    echo "stderr:"
    tput sgr0

    cat $error_file
    echo
  fi
  rm $grammar_file $output_file
}

#
# String newlines are escaped or omitted, because else my auto indentation ruins them.
#

# S → AB
# S → BC
# A → BA
# B → CC
# C → AB
# A → a
# B → b
# C → a
test1_grammar='[ { "alphabet": { "nonterminals": ["A", "B", "S", "C"], "startsymbol": "S", "terminals": ["a", "b"] }, "name": "default", "rules": [ { "LHS": "S", "RHS": ["A", "B"] }, { "LHS": "A", "RHS": ["a"] }, { "LHS": "B", "RHS": ["b"] }, { "LHS": "S", "RHS": ["B", "C"] }, { "LHS": "A", "RHS": ["B", "A"] }, { "LHS": "B", "RHS": ["C", "C"] }, { "LHS": "C", "RHS": ["A", "B"] }, { "LHS": "C", "RHS": ["a"] } ] } ]'
test2_grammar=$test1_grammar

#┏━━━━━━━┯━━━━━┯━━━┯━━━┯━━━┯━━━┓
#┃B      │B    │A,C│B  │A,C│A,C┃
#┠───────┼─────┼───┼───┼───┼───┨
#┃       │S,A  │S,C│S,A│B  │   ┃
#┠───────┼─────┼───┼───┼───┼───┨
#┃A      │S,S,C│B  │   │   │   ┃
#┠───────┼─────┼───┼───┼───┼───┨
#┃S,S,C  │B    │S,A│   │   │   ┃
#┠───────┼─────┼───┼───┼───┼───┨
#┃B      │A,S,A│   │   │   │   ┃
#┠───────┼─────┼───┼───┼───┼───┨
#┃A,A,S,A│     │   │   │   │   ┃
#┗━━━━━━━┷━━━━━┷━━━┷━━━┷━━━┷━━━┛
test1_expected='┏━━━━━━━┯━━━━━┯━━━┯━━━┯━━━┯━━━┓\n┃B      │B    │A,C│B  │A,C│A,C┃\n┠───────┼─────┼───┼───┼───┼───┨\n┃       │S,A  │S,C│S,A│B  │   ┃\n┠───────┼─────┼───┼───┼───┼───┨\n┃A      │S,S,C│B  │   │   │   ┃\n┠───────┼─────┼───┼───┼───┼───┨\n┃S,S,C  │B    │S,A│   │   │   ┃\n┠───────┼─────┼───┼───┼───┼───┨\n┃B      │A,S,A│   │   │   │   ┃\n┠───────┼─────┼───┼───┼───┼───┨\n┃A,A,S,A│     │   │   │   │   ┃\n┗━━━━━━━┷━━━━━┷━━━┷━━━┷━━━┷━━━┛'

#S
#├──B
#│  ├──C
#│  │  ├──A
#│  │  │  ├──B
#│  │  │  │  └──b
#│  │  │  └──A
#│  │  │     ├──B
#│  │  │     │  └──b
#│  │  │     └──A
#│  │  │        └──a
#│  │  └──B
#│  │     └──b
#│  └──C
#│     └──a
#└──C
#   └──a
test2_expected='S\n├──B\n│  ├──C\n│  │  ├──A\n│  │  │  ├──B\n│  │  │  │  └──b\n│  │  │  └──A\n│  │  │     ├──B\n│  │  │     │  └──b\n│  │  │     └──A\n│  │  │        └──a\n│  │  └──B\n│  │     └──b\n│  └──C\n│     └──a\n└──C\n   └──a'

test_case "$test1_grammar" bbabaa cyk "$test1_expected"
test_case "$test2_grammar" bbabaa st  "$test2_expected"

