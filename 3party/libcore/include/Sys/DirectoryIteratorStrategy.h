//
// RecursiveDirectoryIteratorStategies.h
//
// Library: Foundation
// Package: Filesystem
// Module:  RecursiveDirectoryIterator
//
// Definitions of the RecursiveDirectoryIterator stategy classes.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Foundation_RecursiveDirectoryIteratorStrategy_INCLUDED
#define Foundation_RecursiveDirectoryIteratorStrategy_INCLUDED

#include "DirectoryIterator.h"
#include "File.h"

#include <functional>
#include <queue>
#include <stack>

namespace XSpace
{

typedef unsigned short UInt16;

class TraverseBase
{
public:
    typedef std::stack<DirectoryIterator> Stack;
    typedef std::function<UInt16(const Stack &)> DepthFunPtr;

    enum
    {
        D_INFINITE = 0 /// Special value for infinite traverse depth.
    };

    TraverseBase(DepthFunPtr depthDeterminer, UInt16 maxDepth = D_INFINITE);

protected:
    bool isFiniteDepth();
    bool IsDirectory(File &file);

    DepthFunPtr _depthDeterminer;
    UInt16 _maxDepth;
    DirectoryIterator _itEnd;

private:
    TraverseBase();
    TraverseBase(const TraverseBase &);
    TraverseBase &operator=(const TraverseBase &);
};

class ChildrenFirstTraverse : public TraverseBase
{
public:
    ChildrenFirstTraverse(DepthFunPtr depthDeterminer, UInt16 maxDepth = D_INFINITE);

    const std::string next(Stack *itStack, bool *isFinished);

private:
    ChildrenFirstTraverse();
    ChildrenFirstTraverse(const ChildrenFirstTraverse &);
    ChildrenFirstTraverse &operator=(const ChildrenFirstTraverse &);
};

class SiblingsFirstTraverse : public TraverseBase
{
public:
    SiblingsFirstTraverse(DepthFunPtr depthDeterminer, UInt16 maxDepth = D_INFINITE);

    const std::string next(Stack *itStack, bool *isFinished);

private:
    SiblingsFirstTraverse();
    SiblingsFirstTraverse(const SiblingsFirstTraverse &);
    SiblingsFirstTraverse &operator=(const SiblingsFirstTraverse &);

    std::stack<std::queue<std::string>> _dirsStack;
};

} // namespace XSpace

#endif // Foundation_RecursiveDirectoryIteratorStrategy_INCLUDED
