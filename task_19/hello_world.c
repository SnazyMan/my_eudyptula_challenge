#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>

static struct ts_config *conf;

unsigned int snazy_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	int ret;
	struct iphdr *iph;

	if (skb) {
		iph = ip_hdr(skb);
		if (iph->protocol == IPPROTO_TCP) {
			ret = skb_find_text(skb, 0, skb->len, conf);
			if (ret != UINT_MAX)
				pr_debug("9850aeb5fe79\n");
		}
	}
	
	return NF_ACCEPT;
}

static struct nf_hook_ops snazy_nfho = {
	.hook = snazy_hook,
	.hooknum = NF_INET_PRE_ROUTING,
	.pf = PF_INET,
	.priority = NF_IP_PRI_FIRST,

};

static int __init snazy_init(void)
{
	const char *pattern = "9850aeb5fe79";
	conf = textsearch_prepare("kmp", pattern, strlen(pattern), GFP_KERNEL,
				  TS_AUTOLOAD);	

	nf_register_hook(&snazy_nfho);

	return 0;
}
module_init(snazy_init);

static void __exit snazy_exit(void)
{
	nf_unregister_hook(&snazy_nfho);
}
module_exit(snazy_exit);

MODULE_AUTHOR("Tyler Olivieri - Snazyman");
MODULE_DESCRIPTION("task_19 of eudyptula challenge");
MODULE_LICENSE("GPL");
