# Trabalho GB PAD 2023/1

- Grupos de até 3 pessoas
- Implementação do fractal de Mandelbrot
- Processo 1: Pega a matriz e organiza os jobs
	- Job: quadrado ou retângulo com x,y inicial e x,y final
- Processos trabalhadores: pega 1 job, computa o valor do pixel para cada ponto e adiciona na matriz resultado
- Processo impressor: retira 1 resultado, realiza o print na tela
- Possibilitar alterar a complexidade de cores e número de processos trabalhadores

## Compiling and running

```sh
$ make
$ ./fractal
```

---

## References:
- https://physicspython.wordpress.com/2020/02/16/visualizing-the-mandelbrot-set-using-opengl-part-1/
- https://www.brennengreen.dev/blog/posts/1/
- https://github.com/gendestry/Mandelbrot
- http://nuclear.mutantstargoat.com/articles/sdr_fract/
- https://mathworld.wolfram.com/MandelbrotSet.html

## Credits

- Mandelbrot fractal code obtained from [Rosetta Code](https://rosettacode.org/wiki/Mandelbrot_set)
