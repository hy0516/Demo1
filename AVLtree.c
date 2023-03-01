typedef struct BalancedTree{
  struct BalancedTree* left;//左节点
  struct  BalancedTree* right;//右节点
  int data;//数据
  int height;//结点高度
  int bf;//平衡因子
}AVL;

typedef  struct  stacknode        /*链栈存储类型*/
{
  AVL *node;          /*定义结点的数据域*/
  struct  stacknode    *next;    /*定义结点的指针域*/
} LinkStack;
void Push(LinkStack *S,AVL *x)
{  /*进栈函数*/
  LinkStack *p;
  p=(LinkStack *)malloc(sizeof(LinkStack));/*生成新结点*/
  (p->node)=x;     /*将x放入新结点的数据域*/
  p->next=S->next;     /*将新结点插入链表表头之前*/
  S->next=p;          /*新结点作为栈顶*/
}
int EmptyStack(LinkStack *S)
{  /*判断栈空函数*/
  if(S==NULL)           /*栈为空*/
    return 1;
  else
    return 0;
}



int getheight(AVL *T)
//求节点的高度  辅助完成平衡因子的计算
{
  // 请在这里补充代码，完成本关任务
  /********** Begin *********/
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
AVL *RightRotate1(AVL **T){
  //右旋

  AVL *x = (*T)->left;
  AVL *xr = x->right;
  // 旋转
  x->right = (*T);
  (*T)->left = xr;;
  x->height= getheight(x);
  (*T)->height= getheight((*T));
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
LinkStack* L=(LinkStack *)malloc(sizeof(LinkStack));

AVL* AVLDelete( AVL *bt, int k){
  /*在二叉排序树t中删除关键字为k的节点函数*/
  LinkStack* L=(LinkStack *)malloc(sizeof(LinkStack));/*创建链栈存入查找路径*/
  L->next=NULL;
  AVL *p2;
  AVL *p, *f,*s ,*q;
  p=bt;f=NULL;
  while(p)/*查找关键字为k的待删结点p*/
  {   if(p->data==k ){
      break;}    /*找到，则跳出查找循环*/
    f=p;                     /*f指向p结点的双亲结点*/
    if(p->data>k) {
      Push(L, p);/*路径节点入栈*/
      p = p->left;
    }
    else {
      Push(L, p);/*路径节点入栈*/
      p = p->right;
    }
  }
  if(p==NULL) return bt;       /*若找不到，返回原来的二叉排序树*/
  if(p->left==NULL)          /*p无左子树*/
  {  if(f==NULL)               /*p是原二叉排序树的根*/
      bt=p->right;
    else if(f->left==p)     /*p是f的左孩子*/
      f->left=p->right;  /*将p的右子树链到f的左链上*/
    else /*p是f的右孩子*/
      f->right=p->right ; /*将p的右子树链到f的右链上*/
    free(p);                  /*释放被删除的节点p*/
  }
  else /*p有左子树*/
  {   q=p;s=p->left;
    while(s->right)         /*在p的左子树中查找最右下结点*/
    {  q=s;s=s->right;}
    if(q==p) q->left=s->left ;/*将s的左子树链到q左孩子指针*/
    else q->right=s->left;
    p->data=s->data;        /*将s的值赋给p*/
    free(s);
  }
  bt=p2;
  LinkStack *p1=L->next;
  while (p1!=NULL){
    renovate(p1->node);/*更新节点平衡因子*/
    if ((p1->node)->bf>1) {
      if (getheight((p1->node)->left->left) > getheight((p1->node)->left->right)){
          return  RightRotate(p1->node->left);
      }
      else if (getheight((p1->node)->left->left) < getheight((p1->node)->left->right))
        return DoubleTrunLeftRight((p1->node));
    } else if ((p1->node)->bf <-1) {
      if (getheight((p1->node)->right->right) > getheight((p1->node)->right->left))
        return LeftRotate(p1->node);
      else if (getheight((p1->node)->right->left) > getheight((p1->node)->right->right))
        return DoubleTrunRightLeft((p1->node));
    }
    p1=p1->next;
  }

  return bt;
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
