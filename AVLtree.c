typedef struct BalancedTree{
  struct BalancedTree* left;//左节点
  struct  BalancedTree* right;//右节点
  int data;//数据
  int height;//结点高度
  int bf;//平衡因子
}AVL;
//这是一条注释
int getheight(AVL *T)
//求节点的高度  辅助完成平衡因子的计算
{
  if(T==NULL){
    return 0;
  }
  else{
    int  l_height=getheight(T->left);
    int r_height=getheight(T->right);
    return (l_height>r_height?l_height:r_height)+1;
  }
}

int getBF(AVL *T){
  //获取该节点的平衡因子
  if (T==NULL){
    return 0;
  }
  return getheight(T->left)- getheight(T->right);
}
AVL *NewAVL(int k){
  //创建节点
  AVL *t =(AVL* )malloc(sizeof(AVL));
  t->data=k;
  t->height=0;
  t->left=NULL;
  t->right=NULL;
  t->bf=0;
  return t;
}
AVL *RightRotate(AVL *T){
  //右旋
  AVL *x = T->left;
  AVL *xr = x->right;
  // 旋转
  x->right = T;
  T->left = xr;;
  x->height= getheight(x);
  T->height= getheight(T);
  return  x;
}


AVL *LeftRotate(AVL *y){
  //左旋
  AVL *x = y->right;
  x->left=y->right->left;
  x->right=y->right->right;
  x->bf=y->right->bf;
  AVL *xl = x->left;
  // 旋转
  x->left = y;
  y->right = xl;
  // 更新节点的高度
  x->height =getheight(x);
  y->height = getheight(y);
  // 返回旋转后的根节点
  return x;
}

AVL * DoubleTrunLeftRight(AVL *T){
  //LR
  T->left= LeftRotate(T->left);
  return RightRotate(T);

}
AVL * DoubleTrunRightLeft(AVL *T){
  //RL
  T->right= RightRotate(T->right);
  return LeftRotate(T);

}
AVL *insert(AVL *T,int key){
  //平衡二叉树的插入方式
  if (T==NULL){

    return NewAVL(key);
  }
  if (key>T->data){
    T->right= insert(T->right,key);
  }else if (key<T->data){
    T->left= insert(T->left,key);
  }else{
    return  T;
  }
  //更新插入路径每棵子树的高度
  T->height= getheight(T);
  //获取平衡因子 (* 主根节点的bf)
  T->bf= getBF(T);
  //LL型 右旋
  if (T->bf>1 && key<T->left->data){
    return RightRotate(T);
  }else if (T->bf<-1 && key>T->right->data){
    //RR型 左旋
    return LeftRotate(T);
  } else if (T->bf>1 && key>T->left->data){
    //LR型 先左旋后右旋
    T->left= LeftRotate(T->left);
    return RightRotate(T);
  }else if (T->bf<-1 && key<T->right->data){
    //RL型
    T->right= RightRotate(T->right);
    return LeftRotate(T);
  }
  return T;
}
AVL *AVLSearch(AVL *T,int key){
  {  /*二叉排序树的元素查找函数*/
    if (T==NULL){
      printf("查找失败，无该元素");
      return NULL;}  /*空树则返回空指针*/
    else
    {  if (key==T->data)     /*关键字k等于根结点关键字*/
        return T;    /*返回根结点指针*/
      else if(key<T->data)  /*关键字key小于根结点关键字*/
        return(AVLSearch(T->left,key)); /*到根结点的左子树查找*/
      else
        return(AVLSearch(T->right,key)); /*到根结点的右子树查找*/
    }
  }
}
void renovate(AVL *T){
  if (T!=NULL){
    T->height= getheight(T);
    T->bf= getBF(T);
    renovate(T->left);
    renovate(T->right);
  }
}

void display1(AVL *T){
  if (T!=NULL){
    renovate(T);
    printf("%d ----bf=%d\n",T->data,T->bf);
    display1(T->left);
    display1(T->right);
  }
}
AVL *FixBf(AVL *T){ //删除节点后，需要判断是否仍平衡，如果不平衡，则需要调整
  if(T == NULL){
    return NULL;
  }
  if (T->bf>1) {
    if (getheight(T->left->left) > getheight(T->left->right)){
      return  RightRotate(T);
    }
    else if (getheight(T->left->left) < getheight(T->left->right))
      return DoubleTrunLeftRight(T);
  } else if (T->bf <-1) {
    if (getheight(T->right->right) > getheight(T->right->left))
      return LeftRotate(T);
    else if (getheight(T->right->left) > getheight(T->right->right))
      return DoubleTrunRightLeft(T);
  }
  renovate(T);
  return T;
}
AVL *Delete(AVL *T , int x){
  if(T == NULL){
    return NULL;
  }
  if(T->data == x){ //如果找到待删除节点
    if(T->right == NULL){ //如果该节点的右孩子为NULL，那么直接将其删除
      AVL* temp = T;
      T = T->left;
     free(temp);
    }
    else{ //否则，将其右子树的最左孩子作为这个节点，并且递归删除这个节点的值
      AVL *temp;
      temp = T->right;
      while(temp->left){
        temp = temp->left;
      }
      T->data = temp->data;
      T->right = Delete(T->right , T->data);
      renovate(T);//更新平衡节点
    }
    return T;
  }
  if(T->data > x){ //调整删除节点后的节点
    T->left = Delete(T->left , x);
  }
  if(T->data < x){
    T->right = Delete(T->right , x);
  }
  renovate(T);
  T = FixBf(T);
  return T;
}

int main(){
  int arr[]={10,20,30,40,50,25};
  AVL *root=NULL;
  root= insert(root,4);
  root= insert(root,3);
  root= insert(root,2);
  root= insert(root,1);
  root= insert(root,5);
  root= insert(root,6);
  root= insert(root,7);
  root= insert(root,8);
  root= insert(root,10);


  //    root= insert(root,6);
  //    root= insert(root,7);
  //    root= insert(root,8);
  display1(root);
  AVLDelete(root,3);
 printf("==============\n");
  display1(root);
}
