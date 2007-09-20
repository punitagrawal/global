.if !defined(GLOBAL_MAKEFILE_INC_BEEN_HERE)

GLOBAL_MAKEFILE_INC_BEEN_HERE=yes
BINDIR?=	/usr/bin
LDDESTDIR=	-L${.OBJDIR}/../libutil -L${.OBJDIR}/../libdb
LDADD=		-lgloutil -lglodb
DPADD=		${.OBJDIR}/../libutil/libgloutil.a ${.OBJDIR}/../libdb/libglodb.a
CFLAGS+=	-I${.CURDIR} -I${.CURDIR}/../libutil -I${.CURDIR}/../libdb -O
#		-Wall -Wwrite-strings -Wmissing-prototypes
#		-DGTAGSCPP	# alpha test for C++ support.

.endif
