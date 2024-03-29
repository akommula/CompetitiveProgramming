#pragma GCC optimize("Ofast") 
#pragma GCC optimize("unroll-loops") 
#pragma GCC target ("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <string.h>
#include <math.h>

#define INF 200000000000009
#define MAX_N 100005
#define f first
#define s second

using namespace std;

long long ar[MAX_N], pre[MAX_N], pp[MAX_N];
int N, Q;

long long calc(int l, int r, long long laz) {
    return ((r - l + 1) * laz + (r <= 0 ? 0 : pp[r - 1]) - (l <= 1 ? 0 : pp[l - 2]));
}

//ar[i] = ar[i] + x - pre[i - 1] + pre[i - 1]
//ar[i + 1] = ar[i] + x - pre[i - 1] + pre[i]
//ar[i + 2] = ar[i] + x - pre[i - 1] + pre[i + 1] 
//...
//ar[i + y] = ar[i] + x - pre[i - 1] + pre[i + y - 1];

class SegTree {
    private:
        long long lazy[4 * MAX_N];
        long long tree[4 * MAX_N];
        bool active[4 * MAX_N];
    
    public:
        SegTree(long long N) {
        
        }
    
        long long comb(long long a, long long b) {
            return (a + b);
        }
        
        long long get_val(long long l, long long r, long long l1, long long r1, long long ind) {
            if (l1 > r || r1 < l) { return (0); }
            if (l <= l1 && r1 <= r) { return (tree[ind]); }
            if (active[ind]) {
                lazy[2 * ind] = lazy[ind];
                tree[2 * ind] = calc(l1, (r1 + l1) / 2, lazy[ind]);
                active[2 * ind] = true;
        
                lazy[2 * ind + 1] = lazy[ind];
                tree[2 * ind + 1] = calc((r1 + l1) / 2 + 1, r1, lazy[ind]);
                active[2 * ind + 1] = true;
                
                active[ind] = false;
            }
        
            long long leftS = get_val(l, r, l1, (r1 + l1) / 2, (2 * ind));
            long long rightS = get_val(l, r, (r1 + l1) / 2 + 1, r1, 2 * ind + 1);
            
            return (comb(leftS, rightS));
        }

        long long query(long long a, long long b)
        { return (get_val(a, b, 0, MAX_N, 1)); }
        
        void change_val(long long l, long long r, long long newVal, long long ind, long long a, long long b) {
            if (l > b || r < a) { return; }
            if (a <= l && r <= b) {
                lazy[ind] = newVal;
                tree[ind] = calc(l, r, newVal);
                active[ind] = true;
                
                return;
            }
        
            if (active[ind]) {
                lazy[2 * ind] = lazy[ind];
                tree[2 * ind] = calc(l, (r + l) / 2, lazy[ind]);
                active[2 * ind] = true;
            
                lazy[2 * ind + 1] = lazy[ind];
                tree[2 * ind + 1] = calc((r + l) / 2 + 1, r, lazy[ind]);
                active[2 * ind + 1] = true;
            
                active[ind] = false;
            }
        
            change_val(l, (r + l) / 2, newVal, 2 * ind, a, b);
            change_val((r + l) / 2 + 1, r, newVal, 2 * ind + 1, a, b);
            
            tree[ind] = comb(tree[2 * ind], tree[2 * ind + 1]);
        }

        void update(long long a, long long b, long long val)
        { change_val(0, MAX_N, val, 1, a, b); }
};

int main() {
    cin >> N;
    
    SegTree seg = SegTree(MAX_N);

    for (int i = 0; i < N; i ++) {
        cin >> ar[i];
        seg.update(i, i, ar[i]);
    }

    int x;
    for (int i = 0; i < N - 1; i ++) {
        cin >> x;
    
        if (i == 0)
            pre[i] = x;
        else
            pre[i] = pre[i - 1] + x;
    }
    
    for (int i = 0; i < N - 1; i ++) {
        if (i == 0)
            pp[i] = pre[i];
        else
            pp[i] = pp[i - 1] + pre[i];
    }
    
    cin >> Q;
    
    char c;
    int a, b;
    
    for (int i = 0; i < Q; i ++) {
        cin >> c >> a >> b;
        a --;

        if (c == 's')
            cout << seg.query(a, b - 1) << endl; 
        else {
            long long cur = seg.query(a, a);
            
            int low = a;
            int hi = N;

            while (low + 1 < hi) {
                int mid = (low + hi) / 2;
                
                if (cur + b - (a == 0 ? 0 : pre[a - 1]) + (mid == 0 ? 0 : pre[mid - 1]) > seg.query(mid, mid))
                    low = mid;
                else
                    hi = mid;
            }
            
            seg.update(a, low, cur + b - (a == 0 ? 0 : pre[a - 1]));
        }
    }

    return (0);
}