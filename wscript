top = '.'
out = 'build'

from glob import glob
import os


def options(opt):
    opt.load('compiler_cxx waf_unit_test')
    opt.add_option("--docs",
                   action="store_true",
                   default=False,
                   help="Building documentation"
                   )


def configure(cfg):
    cfg.find_program("sphinx-build", var="SPHINXBUILD")
    cfg.load('compiler_cxx waf_unit_test')


def unittest(bld):
    bld.objects(source='unittest/gtest/gtest-all.cc',
                includes='unittest', target='gtest')
    bld.objects(source='unittest/gtest/gtest_main.cc',
                includes='unittest', target='gtest_main')
    for i in glob('unittest/*.cpp'):
        bld.program(
            features='test',
            source=i,
            target=i + ".unittest",
            cxxflags=['-Wall', '-Werror', '-std=c++1y', '-g'],
            use=['gtest', 'gtest_main']
        )
    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)


def build_doc(bld):
    path = {
        'doctrees': os.path.join(out, 'doctrees'),
        'html': os.path.join(out, 'docs', 'html')
    }
    cmdRemoveDoctrees = "rm -rf {0}".format(path['doctrees'])
    cmdSphinxBuild = (
        "${{SPHINXBUILD}} -b html -d {doctrees} docs {html}".format(
            **path))
    bld(
        rule=cmdRemoveDoctrees + " && " + cmdSphinxBuild,
        cwd=bld.path.abspath(),
        source=bld.path.ant_glob('docs/*.py') +
        bld.path.ant_glob('docs/*.rst') +
        bld.path.ant_glob('docs/**/*.rst'),
        target=bld.path.find_or_declare('docs/html/index.html'),
        name='html',
    )


def build(bld):
    if bld.options.docs:
        build_doc(bld)
    bld.add_group()
    unittest(bld)
