#include <iostream>
#include <algorithm>
#include <string>
#include <ctime>
#include <cmath>
#include <cassert>

using namespace std;

template<typename Item>
class IndexMaxHeap{
private:
    Item* data;
    int* indexes;
    int* reverse;
    
    int count;
    int capacity;

    void shiftUp(int k){//the kth value of the indexes list
        while(k>1 && data[indexes[k/2]]<data[indexes[k]]){
            swap(indexes[k/2],indexes[k]);
            reverse[indexes[k/2]] = k/2;
            reverse[indexes[k]] = k;
            k/=2;
        }

    }

    void shiftDown(int k){//the kth value of the indexes list
        while(2*k<= count){//has left child

            //which is bigger, the right or left child. The initial index is the left child
            int j = 2*k;//在此轮循环中，data[k] 和data[j]交换位置
            if(j+1<=count&&data[indexes[j+1]]>data[indexes[j]])//has right child and the right child is bigger
                j+=1;

            if(data[indexes[k]]>=data[indexes[j]])//data[k] should not shiftDown
                break;

            swap(indexes[k],indexes[j]);
            reverse[indexes[k]] = k;
            reverse[indexes[j]] = j;
            k=j;
        }
    }

public:
    IndexMaxHeap(int capacity){
        data = new Item[capacity+1];
        indexes = new int[capacity+1];
        reverse = new int[capacity+1];
        for(int i=0;i<=capacity;i++)
            reverse[i] = 0;
        count = 0;
        this->capacity = capacity;
    }

    ~MaxHeap(){
        delete [] data;
        delete [] indexes;
        delete [] reverse;
    }

    int size(){
        return count;
    }

    bool isEmpty(){
        return count==0;
    }

    // 向最大索引堆中插入一个新的元素, 新元素的索引为i, 元素为item
    // 传入的i对用户而言,是从0索引的
    void insert(int i, Item item){

        assert(count+1<=capacity);
        assert(i+1>=1&&i+1<=capacity);
        i+=1;
        data[i] = item;//id=i  value=item
        indexes[count+1] = i; //insert id=i to the end of max heap

        reverse[i] = count+1;

        count++;
        shiftUp(count);
    }

    Item extractMax(){
        assert(count>0);
        Item ret = data[indexes[1]];

        swap(indexes[1], indexes[count]);
        reverse[indexes[1]] = 1;
        reverse[indexes[count]] = 0;

        count--;
        shiftDown(1);
        return ret;
    }

    int extractMaxIndex(){
        assert(count>0);
        //Item ret = data[indexes[1]];
        int ret = indexes[1] - 1;// -1 covert to index beginning with 0

        swap(indexes[1], indexes[count]);
        reverse[indexes[1]] = 1;
        reverse[indexes[count]] = 0;

        count--;
        shiftDown(1);
        return ret;
    }

    bool contain(int i){
        i += 1;
        assert(i>=1&&i<=capacity);
        return reverse[i]!=0;
    }

    Item getItem(int i){// the index of data
        assert(contain(i));
        return data[i+1];
    }

    void change(int i, Item newItem){//the index of data

        assert(contain(i));

        i+=1;
        data[i] = newItem;

        //find indexes[j] = i, j represents the data[i]'s position on max heap
        //and then shiftUp(j) and shiftDown(j)
//        for(int j=1;i<=count;j++)
//            if(indexes[j]==i){
//                shiftUp(j);
//                shiftDown(j);
//                return;
//            }
        int j = reverse[i];
        shiftDown(j);
        shiftUp(j);

    }

public:
    // 以树状打印整个堆结构
    void testPrint(){

        // 我们的testPrint只能打印100个元素以内的堆的树状信息
        if( size() >= 100 ){
            cout<<"This print function can only work for less than 100 int";
            return;
        }

        // 我们的testPrint只能处理整数信息
        if( typeid(Item) != typeid(int) ){
            cout <<"This print function can only work for int item";
            return;
        }

        cout<<"The max heap size is: "<<size()<<endl;
        cout<<"Data in the max heap: ";
        for( int i = 1 ; i <= size() ; i ++ ){
            // 我们的testPrint要求堆中的所有整数在[0, 100)的范围内
            assert( data[i] >= 0 && data[i] < 100 );
            cout<<data[i]<<" ";
        }
        cout<<endl;
        cout<<endl;

        int n = size();
        int max_level = 0;
        int number_per_level = 1;
        while( n > 0 ) {
            max_level += 1;
            n -= number_per_level;
            number_per_level *= 2;
        }

        int max_level_number = int(pow(2, max_level-1));
        int cur_tree_max_level_number = max_level_number;
        int index = 1;
        for( int level = 0 ; level < max_level ; level ++ ){
            string line1 = string(max_level_number*3-1, ' ');

            int cur_level_number = min(count-int(pow(2,level))+1,int(pow(2,level)));
            bool isLeft = true;
            for( int index_cur_level = 0 ; index_cur_level < cur_level_number ; index ++ , index_cur_level ++ ){
                putNumberInLine( data[index] , line1 , index_cur_level , cur_tree_max_level_number*3-1 , isLeft );
                isLeft = !isLeft;
            }
            cout<<line1<<endl;

            if( level == max_level - 1 )
                break;

            string line2 = string(max_level_number*3-1, ' ');
            for( int index_cur_level = 0 ; index_cur_level < cur_level_number ; index_cur_level ++ )
                putBranchInLine( line2 , index_cur_level , cur_tree_max_level_number*3-1 );
            cout<<line2<<endl;

            cur_tree_max_level_number /= 2;
        }
    }

private:
    void putNumberInLine( int num, string &line, int index_cur_level, int cur_tree_width, bool isLeft){

        int sub_tree_width = (cur_tree_width - 1) / 2;
        int offset = index_cur_level * (cur_tree_width+1) + sub_tree_width;
        assert(offset + 1 < line.size());
        if( num >= 10 ) {
            line[offset + 0] = '0' + num / 10;
            line[offset + 1] = '0' + num % 10;
        }
        else{
            if( isLeft)
                line[offset + 0] = '0' + num;
            else
                line[offset + 1] = '0' + num;
        }
    }

    void putBranchInLine( string &line, int index_cur_level, int cur_tree_width){

        int sub_tree_width = (cur_tree_width - 1) / 2;
        int sub_sub_tree_width = (sub_tree_width - 1) / 2;
        int offset_left = index_cur_level * (cur_tree_width+1) + sub_sub_tree_width;
        assert( offset_left + 1 < line.size() );
        int offset_right = index_cur_level * (cur_tree_width+1) + sub_tree_width + 1 + sub_sub_tree_width;
        assert( offset_right < line.size() );

        line[offset_left + 1] = '/';
        line[offset_right + 0] = '\\';
    }


};

int main() {

    return 0;
}