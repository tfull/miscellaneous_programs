def main(n)
    fs = [File.open("data/data1.txt", "w"), File.open("data/data2.txt", "w")]

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
    if ARGV.length < 1
        puts "wrong number of arguments"
    end
    main(ARGV[0].to_i)
end
