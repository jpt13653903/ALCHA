//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of ALCHA
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//==============================================================================

/**
This is a binary search tree to optimise the scanner.  It facilitates the
greedy match operation.  The tree is only balanced once, so something like
a red-black tree is not required.

It consists of a balanced BST of the first character, each containing a
balanced sub-tree of the next character, etc.                                 */
//------------------------------------------------------------------------------

#ifndef TokenTree_h
#define TokenTree_h
//------------------------------------------------------------------------------

#include "General.h"
#include "Token.h"
//------------------------------------------------------------------------------

class TokenTree{
    private:
        struct Node{
            byte        character; // The character at the current depth
            Token::Type type;      // Unknown => this is not a valid entry

            Node* left;
            Node* right;

            Node* next; // Sub-tree of the next character

            Node(byte chararter);
           ~Node();
        };

        Node* root;

        Node* insert(Node* root, const byte* pattern, Token::Type type);

        // Balancing functions
        Node* balance   (Node* root);
        Node* compress  (Node* root, int count);
        void  subBalance(Node* node);

    public:
        TokenTree();
       ~TokenTree();

        // Insert all the items, then balance it once.  Do not add more items after.
        void insert (const char* pattern, Token::Type type);
        void balance();

        // Finds the longest match and returns the token type and character count
        Token::Type match(const byte* pattern, int* count);

        // Finds an exact match only
        Token::Type find(const byte* pattern);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

