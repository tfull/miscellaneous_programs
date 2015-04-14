def main(n, s1, s2)
    fs = [File.open(s1, "w"), File.open(s2, "w")]

    fs.each do |f|
        f.puts(n)
        for i in 0...n
            for j in 0...n
                a = - 10 + 20 * rand()
                f.print(sprintf("%.2f ", a))
            end
            f.puts("")
        end
    end
end

if $0 == __FILE__
    if ARGV.length != 3
        puts "wrong number of arguments"
    end
    main(ARGV[0].to_i, ARGV[1], ARGV[2])
end
