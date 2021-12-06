from django.core.paginator import Paginator, EmptyPage, PageNotAnInteger
from django.shortcuts import render, get_object_or_404
from django.views.generic import ListView

from .forms import CommentForm, UserAnswerForm
from .models import Post, Poll, PollAnswer


def poll_list(request):
    object_list = Poll.objects.all()

    return render(request, 'blog/poll/poll_list.html', {'polls': object_list})


def poll_detail(request, poll_slug):
    poll = get_object_or_404(Poll, slug=poll_slug)
    poll_answers = poll.answers.all()

    user_answer_form = UserAnswerForm()

    return render(request, 'blog/poll/poll.html',
                  {
                      'poll': poll,
                      'answers': poll_answers,
                      'user_answer_form': user_answer_form
                  })


def save_answer(request):
    object_list = Poll.objects.all()
    poll_answers = PollAnswer.objects.all()

    if request.method == 'POST':
        user_answer_form = UserAnswerForm(data=request.POST)
        if user_answer_form.is_valid():
            new_user_answer = user_answer_form.save(commit=False)

            found_poll_answers = poll_answers.filter(text=new_user_answer.text)
            if found_poll_answers.__len__() > 0:
                firstPollAnswer = found_poll_answers.first()
                new_user_answer.pollAnswer = firstPollAnswer
                new_user_answer.save()

    return render(request, 'blog/poll/poll_list.html', {'polls': object_list})


def post_list(request):
    object_list = Post.objects.all()
    paginator = Paginator(object_list, 3)
    page = request.GET.get('page')
    try:
        posts = paginator.page(page)
    except PageNotAnInteger:
        posts = paginator.page(1)
    except EmptyPage:
        posts = paginator.page(paginator.num_pages)

    return render(request, 'blog/post/list.html', {'page': page, 'posts': posts})


def post_detail(request, year, month, day, post):
    post = get_object_or_404(Post, slug=post, publish__year=year, publish__month=month, publish__day=day)
    comments = post.comments.filter(active=True)
    new_comment = None
    if request.method == 'POST':
        comment_form = CommentForm(data=request.POST)
        if comment_form.is_valid():
            new_comment = comment_form.save(commit=False)
            new_comment.post = post
            new_comment.save()
    else:
        comment_form = CommentForm()
    return render(request, 'blog/post/detail.html',
                  {
                      'post': post,
                      'comments': comments,
                      'new_comment': new_comment,
                      'comment_form': comment_form
                  })


class PostListView(ListView):
    queryset = Post.objects.all()
    context_object_name = 'posts'
    paginate_by = 3
    template_name = 'blog/post/list.html'


class PollListView(ListView):
    queryset = Poll.objects.all()
    context_object_name = 'polls'
    template_name = 'blog/poll/poll_list.html'
