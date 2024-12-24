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

#include "TokenTree.h"
//------------------------------------------------------------------------------

TokenTree::Node::Node(byte character)
{
    left = right = next = 0;

    this->character = character;
    this->type = Token::Type::Unknown;
}
//------------------------------------------------------------------------------

TokenTree::Node::~Node()
{
    if(next ) delete next;
    if(left ) delete left;
    if(right) delete right;
}
//------------------------------------------------------------------------------

TokenTree::TokenTree()
{
    root = 0;
}
//------------------------------------------------------------------------------

TokenTree::~TokenTree()
{
    if(root) delete root;
}
//------------------------------------------------------------------------------

void TokenTree::insert(const char* pattern, Token::Type type)
{
    root = insert(root, (byte*)pattern, type);
}
//------------------------------------------------------------------------------

TokenTree::Node* TokenTree::insert(
    Node*       root,
    const byte* pattern,
    Token::Type type
)
{
    if(!pattern[0]) return root;

    // Keep in vine structure until balancing restructures the tree
    Node* node;
    Node* Prev = 0;
    Node* temp = root;

    while(temp){
        if(*pattern < temp->character){
            node        = new Node(*pattern);
            node->right = temp;
            if(pattern[1]) node->next = insert(node->next, pattern+1, type);
            else           node->type = type;
            if(Prev) Prev->right = node;
            else     root        = node;
            return root;

        }else if(*pattern > temp->character){
            Prev = temp;
            temp = temp->right;

        }else{
            if(pattern[1]){
                temp->next = insert(temp->next, pattern+1, type);
            }else{
                // If temp->type null, this node does not have a token assigned yet
                // If the types are the same, this is an alias, and therefore valid
                if((temp->type != Token::Type::Unknown) && (temp->type != type)){
                    error("Duplicate token entry: ...%s = %d", pattern, (int)type);
                }else{
                    temp->type = type;
                }
            }
            return root;
        }
    }
    node = new Node(*pattern);
    if(pattern[1]) node->next = insert(node->next, pattern+1, type);
    else           node->type = type;
    if(Prev) Prev->right = node;
    else     root        = node;

    return root;
}
//------------------------------------------------------------------------------

/** This balancing operation is based on:
        Quentin F Stout and Bette L Warren,
        "Tree rebalancing in optimal space and time"
        Communications of the ACM, September 1986, Volume 29, Number 9
        https://web.eecs.umich.edu/~qstout/pap/CACM86.pdf
        https://deepblue.lib.umich.edu/bitstream/handle/2027.42/7801/bad3920.0001.001.pdf?sequence=5&isAllowed=y */

void TokenTree::balance()
{
    root = balance(root);
}
//------------------------------------------------------------------------------

TokenTree::Node* TokenTree::balance(Node* root)
{
    if(!root) return 0;

    // count the items in the vine
    int   count = 0;
    Node* node  = root;
    while(node){
        count++;
        node = node->right;
    }

    // Create the deepest leaves
    int j    = 0x8000;
    int Size = count + 1;
    while(j > Size) j >>= 1; // j = 2^floor(log2(count + 1))
    Size -= j;

    if(Size) root = compress(root, Size);
    Size = count - Size;

    // balance the tree
    while(Size > 1){
        Size /= 2;
        root  = compress(root, Size);
    }

    subBalance(root);

    return root;
}
//------------------------------------------------------------------------------

TokenTree::Node* TokenTree::compress(Node* root, int count)
{
    Node* temp  = root->right;
    root->right = temp->left;
    temp->left  = root;
    root        = temp;

    int   j;
    Node* node = root;

    for(j = 1; j < count; j++){
        temp               = node->right->right;
        node->right->right = temp->left;
        temp->left         = node->right;
        node->right        = temp;
        node               = temp;
    }
    return root;
}
//------------------------------------------------------------------------------

void TokenTree::subBalance(Node* node)
{
    if(node->next) node->next = balance(node->next);

    if(node->left ) subBalance(node->left );
    if(node->right) subBalance(node->right);
}
//------------------------------------------------------------------------------

Token::Type TokenTree::match(const byte* pattern, int* count)
{
    int         n    = 0;
    Token::Type type = Token::Type::Unknown;

    *count = 0;

    Node* node = root;

    while(node){
        if(*pattern < node->character){
            node = node->left;

        }else if(*pattern > node->character){
            node = node->right;

        }else{
            n++;
            if(node->type != Token::Type::Unknown){ // Keep track of the best option
                *count = n;
                type   = node->type;
            }
            if(pattern[1]){
                pattern++;
                node = node->next;
            }else{
                return type;
            }
        }
    }
    return type;
}
//------------------------------------------------------------------------------

Token::Type TokenTree::find(const byte* pattern)
{
    Node* node = root;

    while(node){
        if(*pattern < node->character){
            node = node->left;

        }else if(*pattern > node->character){
            node = node->right;

        }else{
            if(pattern[1]){
                pattern++;
                node = node->next;
            }else{
                return node->type;
            }
        }
    }
    return Token::Type::Unknown;
}
//------------------------------------------------------------------------------
