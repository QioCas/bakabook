
// f = a * x + b
// update(f)
// query(pos, kth)

struct liChao {
    vector<vector<line>> tr;
    int mkth;

    liChao(int sz, int k) : tr(4 * sz), mkth(k) {
    }

    void update(line cur, int k, int l, int r) {
        /*
            cur: đường thẳng cần cập nhật
            k: nút hiện tại
            l..r: khoảng mà nút quản lý
        */
        tr[k].push_back(cur);
        if (tr[k].size() <= 2 * mkth - 1)
            return;

        // chèn đường thẳng mới vào và đưa đường thẳng tệ nhất ra ngoài
        int mid = (l + r) >> 1;
        nth_element(tr[k].begin(), tr[k].end() - 1, tr[k].end(),
                    [&](line a, line b) { return a.calc(mid) < b.calc(mid); }); // đưa đường thẳng tệ nhất ra cuối vector
        line worst = tr[k].back();
        tr[k].pop_back();

        if (l == r)
            return;

        // đếm số đường thẳng được lưu ở nút hiện tại mà tốt hơn đường thẳng tệ nhất
        // trong khoảng [l; mid], bằng cách so sánh hệ số góc của chúng
        int dominateLeft = 0;
        for (line it : tr[k])
            dominateLeft += it.slope() > worst.slope();

        // đường thẳng tệ nhất được đưa xuống cây con có
        // số đường thẳng tốt hơn nó ít hơn "mkth"
        if (dominateLeft < mkth)
            update(worst, 2 * k, l, mid);
        else
            update(worst, 2 * k + 1, mid + 1, r);
    }

    void push(vector<ll> &cand, ll val, int kth) {
        // thêm một giá trị vào vector và giữ lại k giá trị tốt nhất
        cand.push_back(val);
        if (cand.size() > kth) {
            nth_element(cand.begin(), cand.end() - 1, cand.end());
            cand.pop_back();
        }
    }

    void walk(vector<ll> &cand, int pos, int kth, int k, int l, int r) {
        /*
            cand: vector để duy trì k giá trị tốt nhất
            pos: vị trí được truy vấn
            kth: thứ hạng của đường thẳng cần tìm
            k: nút hiện tại
            l..r: khoảng mà nút quản lý
        */
        for (line cur : tr[k])
            push(cand, cur.calc(pos), kth);
        if (l == r)
            return;

        int mid = (l + r) >> 1;
        if (pos <= mid)
            walk(cand, pos, kth, 2 * k, l, mid);
        else
            walk(cand, pos, kth, 2 * k + 1, mid + 1, r);
    }

    ll query(int pos, int kth, int k, int l, int r) {
        /*
            pos: vị trí được truy vấn
            kth: thứ hạng của đường thẳng cần tìm
            k: nút hiện tại
            l..r: khoảng mà nút quản lý
        */
        vector<ll> cand;
        walk(cand, pos, kth, k, l, r);
        nth_element(cand.begin(), cand.end() - 1, cand.end());
        return cand.back();
    }
};
