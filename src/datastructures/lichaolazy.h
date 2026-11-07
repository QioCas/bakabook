// f = a * x + b
// update(l, r, f) a[i] = min(a[i], f.a * i + b)
// update(l, r, a, b) a[i] += a * i + b
// query(pos)

struct line {
    ll a, b;

    line() : a(0), b(0) {
    }
    line(ll a, ll b) : a(a), b(b) {
    }

    ll calc(ll x) {
        return (b == LLONG_MAX ? LLONG_MAX : a * x + b);
    }
    ll slope() {
        return a;
    }

    ll add(ll a, ll b) { // phép cộng chống tràn số
        return (max(a, b) == LLONG_MAX ? LLONG_MAX : a + b);
    }

    void operator+=(line o) {
        a = add(a, o.a), b = add(b, o.b);
    }
};

struct liChao {
    vector<line> tr, lazy;

    liChao() {
    }
    liChao(int sz) : tr(4 * sz, line(0, LLONG_MAX)), lazy(4 * sz) {
    }

    void apply(int k, line f) {
        tr[k] += f, lazy[k] += f;
    }

    void pushDown(int k) {
        // đẩy "hàm cập nhật" theo cập nhật tăng đoạn xuống các cây con
        // để tránh ảnh hưởng của cập nhật chèn đường thẳng
        if (!lazy[k].a && !lazy[k].b)
            return;
        apply(2 * k, lazy[k]), apply(2 * k + 1, lazy[k]);
        lazy[k] = line(0, 0);
    }

    void insertLine(line f, int k, int l, int r) {
        /*
            f: đường thẳng cần cập nhật
            k: nút hiện tại
            l..r: khoảng mà nút quản lý
        */

        // thực hiện cập nhật đường thẳng như Li-chao tree cơ bản, thêm
        // thao tác "pushDown" để không cập nhật đè lên các cập nhật tăng đoạn
        if (l == r) {
            tr[k] = (f.calc(l) < tr[k].calc(l) ? f : tr[k]);
            return;
        }
        pushDown(k);
        int mid = (l + r) >> 1;
        if (f.calc(mid) < tr[k].calc(mid))
            swap(tr[k], f);
        if (f.slope() > tr[k].slope())
            insertLine(f, 2 * k, l, mid);
        else if (f.slope() < tr[k].slope())
            insertLine(f, 2 * k + 1, mid + 1, r);
    }

    void pushLine(int k, int l, int r) {
        // đẩy hàm của Li-chao tree xuống các cây con để tránh
        // ảnh hưởng của cập nhật tăng đoạn
        int mid = (l + r) >> 1;
        if (tr[k].b != LLONG_MAX) {
            insertLine(tr[k], 2 * k, l, mid);
            insertLine(tr[k], 2 * k + 1, mid + 1, r);
            tr[k] = line(0, LLONG_MAX);
        }
    }

    void updateInsert(int a, int b, line f, int k, int l, int r) {
        /*
            a..b: khoảng cần cập nhật
            f: đường thẳng cần cập nhật
            k: nút hiện tại
            l..r: khoảng mà nút quản lý
        */

        // thực hiện cập nhật đoạn thẳng như Li-chao tree cơ bản, thêm
        // thao tác "pushDown" để không cập nhật đè lên các cập nhật tăng đoạn
        if (b < l || r < a)
            return;
        if (a <= l && r <= b)
            return insertLine(f, k, l, r), void();
        pushDown(k);
        int mid = (l + r) >> 1;
        updateInsert(a, b, f, 2 * k, l, mid);
        updateInsert(a, b, f, 2 * k + 1, mid + 1, r);
    }

    void updateIncrease(int a, int b, line f, int k, int l, int r) {
        /*
            a..b: khoảng cần cập nhật
            f: đường thẳng cần cập nhật
            k: nút hiện tại
            l..r: khoảng mà nút quản lý
        */

        // thực hiện cập nhật đoạn tương tự Segment tree, thêm thao tác
        // "pushDown" và "pushLine" để không cập nhật đè lên các cập nhật trước đó
        if (b < l || r < a)
            return;
        if (a <= l && r <= b)
            return apply(k, f), void();
        pushDown(k), pushLine(k, l, r);
        int mid = (l + r) >> 1;
        updateIncrease(a, b, f, 2 * k, l, mid);
        updateIncrease(a, b, f, 2 * k + 1, mid + 1, r);
    }

    ll query(int pos, int k, int l, int r) {
        /*
            pos: vị trí được truy vấn
            k: nút hiện tại
            l..r: khoảng mà nút quản lý
        */
        ll cur = tr[k].calc(pos);
        int mid = (l + r) >> 1;
        if (l == r)
            return cur;
        pushDown(k);
        if (pos <= mid)
            return min(cur, query(pos, 2 * k, l, mid));
        else
            return min(cur, query(pos, 2 * k + 1, mid + 1, r));
    }
};