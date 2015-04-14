type tree =
  | Num of int
  | Add of tree * tree
  | Sub of tree * tree
  | Mul of tree * tree
  | Div of tree * tree
  | Mod of tree * tree
    
let rec make_random_tree lim =
  if lim > 0 then
    if Random.int 100 < 30 then Num (Random.int 100)
    else
      match Random.int 100 with
        | x when x <  20 -> Add (make_random_tree (lim-1), make_random_tree (lim-1))
        | x when x <  40 -> Sub (make_random_tree (lim-1), make_random_tree (lim-1))
        | x when x <  60 -> Mul (make_random_tree (lim-1), make_random_tree (lim-1))
        | x when x <  80 -> Div (make_random_tree (lim-1), make_random_tree (lim-1))
        | _ -> Mod (make_random_tree (lim-1), make_random_tree (lim-1))
  else
    Num (Random.int 100)

let rec string_of_tree = function
  | Num n -> string_of_int n
  | Add (t1, t2) -> "(" ^ string_of_tree t1 ^ " + " ^ string_of_tree t2 ^ ")"
  | Sub (t1, t2) -> "(" ^ string_of_tree t1 ^ " - " ^ string_of_tree t2 ^ ")"
  | Mul (t1, t2) -> "(" ^ string_of_tree t1 ^ " * " ^ string_of_tree t2 ^ ")"
  | Div (t1, t2) -> "(" ^ string_of_tree t1 ^ " / " ^ string_of_tree t2 ^ ")"
  | Mod (t1, t2) -> "(" ^ string_of_tree t1 ^ " % " ^ string_of_tree t2 ^ ")"
    
let _ =
  let depth = if Array.length Sys.argv >= 2 then int_of_string Sys.argv.(1) else 5 in
  let tree = Random.self_init (); make_random_tree depth in
  let s = string_of_tree tree in
  print_string (s ^ "\n")
