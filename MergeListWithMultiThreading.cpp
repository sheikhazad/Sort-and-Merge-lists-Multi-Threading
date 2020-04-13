/*list<int> l1{0, 3, 6, 9, 12, 15};
list<int> l2{1, 4, 7, 10};
list<int> l3{2, 5, 8, 8};
list<int> l4{1, 2, 6, 9, 16};
list<int> l5{3, 5, 7, 9, 17, 19, 20};
*/

#include<list>
#include<iostream>
#include<thread>
#include<future>
#include<queue>

using namespace std;

list<int> mergeLists(const list<int>& l1, const list<int>& l2)
{
  if(l1.empty())
    return l2;
  if(l2.empty())
    return l1;

  list<int> l3;
  list<int>::const_iterator it1 = l1.cbegin();
  list<int>::const_iterator it2 = l2.cbegin();

  while( (it1 != l1.cend()) && (it2 != l2.cend()))
  {
     if(*it1 < *it2)
     {
       l3.emplace_back(*it1);
       ++it1;
     }
     else
     {
       l3.emplace_back(*it2);
       ++it2;
     }
  }
  while(it1 != l1.cend())
  {
     l3.emplace_back(*it1);
       ++it1;
  }
  while(it2 != l2.cend())
  {
     l3.emplace_back(*it2);
       ++it2;
  }
  return l3;
}

list<int> mergeMultiThread(const list<list<int>>& lists){

    if (lists.empty()) return list<int>();
    if (lists.size() == 2) return mergeLists(*lists.begin(), *lists.rbegin());

    queue<future<list<int>>> Q;

    auto iter = lists.cbegin(); ++iter;
    auto iterPrev = lists.cbegin();

    for( ; iter != lists.end() && iterPrev != lists.end(); std::advance(iter,2), std::advance(iterPrev,2)){
        Q.emplace(async(std::launch::async | std::launch::deferred, mergeLists, *iterPrev, *iter));
    }

    if ( iterPrev != lists.end()){
        Q.emplace(async(std::launch::async | std::launch::deferred, mergeLists, *iterPrev, list<int>()));
    }

    while(!Q.empty()){
        future<list<int>> fuLis1 {std::move(Q.front())};
        Q.pop();

        if(Q.empty()){
            return fuLis1.get();
        }

        future<list<int>> fuList2 {std::move(Q.front())};
        Q.pop();

        Q.emplace(async(std::launch::async | std::launch::deferred, mergeLists, fuLis1.get(), fuList2.get()));
    }

    return list<int>();
}
int main()
{
    list<int> l1{0, 3, 6, 9, 12, 15};
    list<int> l2{1, 4, 7, 10};
    list<int> l3{2, 5, 8, 8};
    list<int> l4{1, 2, 6, 9, 16};
    list<int> l5{3, 5, 7, 9, 17, 19, 20};

    cout<<"******COMPARE NON-THREAD RESULT & MULTI-THREAD RESULT********" << endl;
    cout<<"*************************************************************" <<endl;
    cout<<"------------TEST CASE-1: l1+l2 ------------------------" <<endl <<endl;
    cout<< "Result from NON-THREAD Flow : " << endl;
    list<int> LR1 = mergeLists(l1,l2);
    for(auto r: LR1)
    {
        cout<< r <<" ";
    }
    cout<< "\n\nResult from MULTI-THREADING: " << endl;
    for(auto r3: mergeMultiThread({l1,l2}))
    {
        cout<< r3 <<" ";
    }

    cout<<"\n\n------------TEST CASE-2: l1+l2+l3 ------------------" <<endl<<endl;
    cout<< "Result from NON-THREAD Flow : " << endl;
    list<int> LR2 = mergeLists(LR1,l3);
    for(auto r: LR2)
    {
        cout<< r <<" ";
    }
    cout<< "\n\nResult from MULTI-THREADING: " << endl;
    for(auto r3: mergeMultiThread({l1,l2,l3}))
    {
        cout<< r3 <<" ";
    }

    cout<<"\n\n------------TEST CASE-3: l1+l2+l3+l4 --------------" <<endl<<endl;
    cout<< "Result from NON-THREAD Flow : " << endl;
    list<int> LR3 = mergeLists(LR2,l4);
    for(auto r: LR3)
    {
        cout<< r <<" ";
    }
    cout<< "\n\nResult from MULTI-THREADING: " << endl;
    for(auto r3: mergeMultiThread({l1,l2,l3,l4}))
    {
        cout<< r3 <<" ";
    }

    cout<<"\n\n------------TEST CASE-4: l1+l2+l3+l4+l5 -----------" <<endl<<endl;
    cout<< "Result from NON-THREAD Flow : " << endl;
    list<int> LR4 = mergeLists(LR3,l5);
    for(auto r: LR4)
    {
        cout<< r <<" ";
    }

    cout<< "\n\nResult from MULTI-THREADING: " << endl;
    for(auto r3: mergeMultiThread({l1,l2,l3,l4,l5}))
    {
        cout<< r3 <<" ";
    }
    cout << endl;

    cout<<"\n------------TEST CASE-5: l1 - Single List--------------------" <<endl;
    cout<< "Result from NON-THREAD Flow : " << endl;
    list<int>dummy;
    list<int> LR5 = mergeLists(l1, dummy);
    for(auto r: LR5)
    {
        cout<< r <<" ";
    }
    cout<< "\nResult from MULTI-THREADING:  - WHEN SINGLE LIST : " << endl;
    for(auto r3: mergeMultiThread({l1}))
    {
        cout<< r3 <<" ";
    }
}






