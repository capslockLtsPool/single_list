/**
 *
 * I grub it from linux kernel source code and fix it for user space
 * program. However, this is a BSD licensed header file.
 *
 * - Leo Ma  2014.01.29
 *
*/

#ifndef _S_LINK_H_
#define _S_LINK_H_

#define LINK_POISION ((void *)0x00100100)

struct s_link {
  struct s_link *next;
};

#define link_entry(pos, type, member) \
  ((type *)((char *)(pos)- ((size_t)&((type *)0)->member)))

#define link_first_entry(pos, type, member) \
  link_entry((pos)->next, type, member)

#define link_foreach(pos, head) \
  for (pos = (head)->next; pos != (head); pos = (pos)->next)

#define link_foreach_safe(pos, n, head) \
  for (pos = (head)->next, n = (pos)->next; pos != (head); pos = n, n = (pos)->next)

static inline void
link_init(struct s_link *link)
{
  link->next = link;
}

static inline int
link_empty(const struct s_link *head)
{
  return head->next == head;
}

static inline int
link_is_last(const struct s_link *pos, const struct s_link *head)
{
  return pos->next == head;
}

static inline int
link_is_singular(const struct s_link *head)
{
  return !link_empty(head) && head->next->next == head;
}

static inline struct s_link *
link_forward_until(struct s_link *pos, struct s_link *prev)
{
  while (prev->next != pos) {
    prev = prev->next;
	}
  return prev;
}

static inline void
__link_add(struct s_link *prev, struct s_link *new)
{
  new->next = prev->next;
  prev->next = new;
}

static inline void
link_add(struct s_link *new, struct s_link *head)
{
  __link_add(head, new);
}

static inline void
link_add_tail(struct s_link *new, struct s_link *head)
{
  __link_add(link_forward_until(head, head), new);
}

static inline void
__link_del(struct s_link *prev, struct s_link *pos)
{
  prev->next = pos->next;
}

static inline void
link_del(struct s_link *pos, struct s_link *head)
{
  __link_del(link_forward_until(pos, head), pos);
  pos->next = LINK_POISION;
}

static inline void
link_del_init(struct s_link *pos, struct s_link *head)
{
  __link_del(link_forward_until(pos, head), pos);
  link_init(pos);
}

static inline void
link_replace(struct s_link *old, struct s_link *new, struct s_link *head)
{
  struct s_link *prev;
  prev = link_forward_until(old, head);
  new->next = old->next;
  prev->next = new;
  old->next = LINK_POISION;
}

static inline void
link_replace_init(struct s_link *old, struct s_link *new, struct s_link *head)
{
  struct s_link *prev;
  prev = link_forward_until(old, head);
  new->next = old->next;
  prev->next = new;
  link_init(old);
}

static inline void
link_move(struct s_link *pos, struct s_link *head, struct s_link *new_head)
{
  link_del(pos, head);
  link_add(pos, new_head);
}

static inline void
link_move_tail(struct s_link *pos, struct s_link *head, struct s_link *new_head)
{
  link_del(pos, head);
  link_add_tail(pos, new_head);
}

#endif /* _S_LINK_H_ */
