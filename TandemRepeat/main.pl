repeats(W,WORDS) :- concat_atom([PWW,S],WORDS), concat_atom([P,WW],PWW), concat_atom([W,W],WW), W \== ''.
maximum_repeat(W,WORDS) :- findall(X,repeats(X,WORDS),L), maplist(atom_length,L,Ns), m_list(Ns,M), member(W,L), atom_length(W,M).
m_list(L,M) :- list_to_set(L, Lhh), select(M,Lhh,Lh), forall(member(E,Lhh),E =< M).
