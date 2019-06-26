import sys
import glob
import subprocess
import os
import imageio


def get_all_visualized_filename(out_filename, path):
    return glob.glob('{}*_{}'.format(path, out_filename))


def create_gif(result_filename, out_filenames, duration):
    images = []
    for filename in out_filenames:
        images.append(imageio.imread(filename))
    output_file = '{}.gif'.format(result_filename)
    imageio.mimsave(output_file, images, duration=duration)


def main(result_filename, iteration_filename, path=''):
    iter_filenames = get_all_visualized_filename(iteration_filename, path)
    iter_filenames.sort(key=(lambda x: int(x.split('_')[0].split('/')[-1])))

    create_gif(result_filename, iter_filenames, 0.5)


if __name__ == "__main__":
    main('visualized/cold_edges__hot_middle_1000', 'iter.jpg')
