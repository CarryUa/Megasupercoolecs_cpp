#pragma once

namespace msce
{

template <typename T> class BinaryTreeNode
{
private:
  T data_;
  BinaryTreeNode<T> *left_ = nullptr;
  BinaryTreeNode<T> *right_ = nullptr;
  BinaryTreeNode<T> *parent_ = nullptr;

public:
  BinaryTreeNode(T data);
  BinaryTreeNode(T data, BinaryTreeNode<T> *parent, BinaryTreeNode<T> *left,
                 BinaryTreeNode<T> *right);

  void left(BinaryTreeNode<T> *l);
  BinaryTreeNode<T> *left() const;

  void right(BinaryTreeNode<T> *r);
  BinaryTreeNode<T> *right() const;

  void parent(BinaryTreeNode<T> *p);
  BinaryTreeNode<T> *parent() const;

  void data(T data);
  T data() const;
};

template <typename T>
inline BinaryTreeNode<T>::BinaryTreeNode(T data) : data_(data)
{
}

template <typename T>
inline BinaryTreeNode<T>::BinaryTreeNode(T data, BinaryTreeNode<T> *parent,
                                         BinaryTreeNode<T> *left,
                                         BinaryTreeNode<T> *right)
    : data_(data), parent_(parent), left_(left), right_(right)
{
  if (left_) left_->parent(this);

  if (right_) right_->parent(this);
}

template <typename T> inline void BinaryTreeNode<T>::left(BinaryTreeNode<T> *l)
{
  left_ = l;
}
template <typename T> inline BinaryTreeNode<T> *BinaryTreeNode<T>::left() const
{
  return left_;
}
template <typename T> inline void BinaryTreeNode<T>::right(BinaryTreeNode<T> *r)
{
  right_ = r;
}
template <typename T> inline BinaryTreeNode<T> *BinaryTreeNode<T>::right() const
{
  return right_;
}
template <typename T>
inline void BinaryTreeNode<T>::parent(BinaryTreeNode<T> *p)
{
  parent_ = p;
}
template <typename T>
inline BinaryTreeNode<T> *BinaryTreeNode<T>::parent() const
{
  return parent_;
}
template <typename T> inline void BinaryTreeNode<T>::data(T data)
{
  data_ = data;
}
template <typename T> inline T BinaryTreeNode<T>::data() const { return data_; }
} // namespace msce