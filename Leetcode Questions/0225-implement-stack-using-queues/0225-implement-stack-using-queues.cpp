class MyStack {
public:
    queue<int> q1;
    queue<int> q2;
    MyStack() {
        
    }
    
    void push(int x) {
        if(q1.empty()){
            q1.push(x);
            while(!q2.empty()){
                q1.push(q2.front());
                q2.pop();
            }
        }
        else{
            q2.push(x);
            while(!q1.empty()){
                q2.push(q1.front());
                q1.pop();
            }
        }
      }
    
    
    int pop() {
        if(q2.empty()){
            int temp_1 = q1.front();
             q1.pop();
             return temp_1;
        }
        else if (q1.empty()){
            int temp_2 = q2.front();
             q2.pop();
             return temp_2;
        }
        return -1;
    }
    
    int top() {
       if (q1.empty()){
           return q2.front();
       }
       if(q2.empty()){
           return q1.front();
       } 
       return -1;
    }
    
    bool empty() {
      return(q1.empty() && q2.empty());  
    }
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack* obj = new MyStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * bool param_4 = obj->empty();
 */