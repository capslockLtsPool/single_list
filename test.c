#include <stdio.h>
#include "s_link.h"

struct person {
	struct s_link clothes;
	struct s_link trousers;
};

struct clothes {
	struct s_link link;
	int i;
};

struct trouser {
	struct s_link link;
	int i;
};

int main(int argc, char **argv)
{
	struct s_link *link, *nlink;
	struct person p;
	struct clothes cl[10];
	struct trouser tr[10];
	int i;

	for (i = 0; i < 10; i++) {
	  cl[i].i = i;
	  tr[i].i = i;
	}

	link_init(&p.clothes);
	link_init(&p.trousers);

	link_add(&cl[4].link, &p.clothes);
	link_add(&cl[8].link, &p.clothes);
	link_add(&tr[5].link, &p.trousers);
	link_replace(&tr[5].link, &tr[6].link,&p.trousers);

	link_foreach(link, &p.clothes) {
		struct clothes *cloth = link_entry(link, struct clothes, link);
		printf("cl:%d\n", cloth->i);
	}

	link_foreach(link, &p.trousers) {
		struct trouser *trous = link_entry(link, struct trouser, link);
		printf("tr:%d\n", trous->i);
	}

	link_move(&cl[8].link, &p.clothes, &p.trousers);
	
	link_foreach(link, &p.clothes) {
		struct clothes *cloth = link_entry(link, struct clothes, link);
		printf("cl:%d\n", cloth->i);
	}

	link_foreach(link, &p.trousers) {
		struct trouser *trous = link_entry(link, struct trouser, link);
		printf("tr:%d\n", trous->i);
	}

	printf("singular:%d\n", link_is_singular(&p.clothes));

	link_foreach_safe(link, nlink, &p.trousers)
	{
		printf("is_last:%d\n", link_is_last(link, &p.trousers));
		link_del(link, &p.trousers);
	}

	printf("empty:%d\n", link_empty(&p.trousers));

  return 0;
}
