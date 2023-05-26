# Sequence

Given a sequence $A$ of positive integers of length $N$. 

Define $W(l,r,x)$ as $\sum\limits_{i=l}^r\mathbb{I}[A[i]=x]$ i.e. the number of occurrences of $x$ in $A[l]\dots A[r]$. 

Define the **median** of a sequence $B[1],B[2]\dots B[k]$ as $S(\{B[1],B[2]\dots B[k]\})$:

First sort $B[1],B[2]\dots B[k]$ in ascending order to obtain sequence $C[1],C[2],\dots C[k]$.

Then $S(\{B[1],B[2]\dots B[k]\})=\{C[{\lfloor\frac k 2\rfloor}], C[{\lceil\frac k 2\rceil}]\}$. 

Note that $S(\{B[1],B[2]\dots B[k]\})$ is a set of integers.

For example, $S(\{6,3,5,4,6,2,3\})=\{4\}$, $S(\{4,2,3,1\})=\{2,3\}, $ $S(\{5,4,2,4\})=\{4\}$.

For a pair of integers $l, r(0\leq l\leq r\leq n-1)$, define $S(l,r)$ as the **median** of $A[l]\dots A[r]$. Furthermore, we define the value of $(l,r)$ as $\max\limits_{x\in S(l,r)}W(l,r,x)$. 


Alice wants to find the maximum value among all possible pairs of integers $(l,r)$ that satisfy $0\leq l\leq r\leq n-1$.

## Implementation Details

You should implement the following procedure:

`int sequence(int N, std::vector<int> A)`

- $N$: the length of sequence $A$.


- $A$: array of length $N$, describing the sequence $A$.


- This procedure  should return an integer representing the maximum value among all possible pairs $(l,r)$.
- This procedure is called exactly once.

## Example

### Example 1

Consider the following call:

`sequence(7,{1 2 3 1 2 1 3})`

This procedure should return $3$.

In this case, $S(0,5)=\{1,2\}$, $W(0,5,1)=3$, $W(0,5,2)=2$. So the value of $(0,5)$ is $3$.

It is easy to verify that $(0,5)$ has the greatest value among all possible pairs.

This example satisfies subtask $1,2,4,6,7$.

### Example 2

Consider the following call:

`sequence(9,{1 1 2 3 4 3 2 1 1})`

This procedure should return $2$.

This example satisfies subtask $1,2,3,6,7$.

### Example 3

Consider the following call:

`sequence(14,{2 6 2 5 3 4 2 1 4 3 5 6 3 2}`)

This procedure should return $3$.

This example satisfies subtask $1,2,6,7$.

## Constraints

- $1\leq N\leq 5\times 10^5$
- $1\leq A_i\leq N$

## Subtasks

1.(11 points): $N\leq 100$.

2.(17 points): $N\leq 2\times 10^3$.

3.(7 points): There exists an $x$ that satisfy  $\forall 0\leq i<x,A[i]\leq A[i+1]$ and  $\forall x<i<N, A[i]\leq A[i-1]$.

4.(12 points): $A[i]\leq 3$.

$5.$(13 points): $W(0,N-1,A[i])\le 2$ (for each $i$ such that $0\le i \le N-1$)

$6.$(22 points): $N\leq 8\times 10^4$.

$7.$(18 points): No additional constraints.

## Sample grader

The sample grader reads input in the following format:

Line $1$: $N$

Line $2$: $A[0]\ A[1]\ \dots\ A[N-1]$.

The sample grader prints your output in the following format:

Line $1$: the return value of sequence.

