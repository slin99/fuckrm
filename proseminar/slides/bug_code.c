    static int ptype_seq_show(...){
        ...
        if (pt->dev == NULL || dev_net(pt->dev)==seq_file_net(seq)) {
            if (pt->type == htons(ETH_P_ALL))
                seq_puts(seq,"ALL ");
            else
        }
        ...
        }