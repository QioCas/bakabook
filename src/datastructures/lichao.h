// f = a * x + b
// update(l, r, f)
// query(pos)

struct liChao {
    vector<line> tr;

    liChao() {
    }
    liChao(int sz) : tr(4 * sz, line(0, LLONG_MAX)) {
    }

    void addLine(line f, int k, int l, int r) {
        /*
            f: đường thẳng cần cập nhật
            k: nút hiện tại
            l..r: khoảng mà nút quản lý
        */
        if (l == r) {
            tr[k] = (f.calc(l) < tr[k].calc(l) ? f : tr[k]);
            return;
        }
        int mid = (l + r) >> 1;
        if (f.calc(mid) < tr[k].calc(mid))
            swap(tr[k], f);
        if (f.slope() > tr[k].slope())
            addLine(f, 2 * k, l, mid); // trường hợp 1.1
        if (f.slope() < tr[k].slope())
            addLine(f, 2 * k + 1, mid + 1, r); // trường hợp 1.2
    }

    void update(int a, int b, line f, int k, int l, int r) {
        /*
            a..b: khoảng cần cập nhật
            f: đường thẳng cần nhập nhật
            k: nút hiện tại
            l..r: khoảng mà nút quản lý
        */
        if (b < l || r < a)
            return;

        // thực hiện thao tác cập nhật của Li-chao tree cơ bản
        // khi khoảng mà nút quản lý nằm gọn trong khoảng cần cập nhật
        if (a <= l && r <= b)
            return addLine(f, k, l, r), void();

        // chia để trị tương tự Segment tree
        int mid = (l + r) >> 1;
        update(a, b, f, 2 * k, l, mid);
        update(a, b, f, 2 * k + 1, mid + 1, r);
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
        if (pos <= mid)
            return min(cur, query(pos, 2 * k, l, mid));
        else
            return min(cur, query(pos, 2 * k + 1, mid + 1, r));
    }
};
