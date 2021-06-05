#include "../includes/ft_ssl.h"

static const char *error_messages[5] =
{
        "Can't allocate memory",
        "First argument should be encrypt mode, but has",
        "No such file or directory",
        "usage: ft_ssl command [command opts] [command args]",
        "md5: option requires an argument -- s"
};

static void print_wrong_command(t_env *env, t_data *data)
{
    write(1, "ft_ssl: ", ft_strlen("ft_ssl: "));
    write (1, env->hashname_lowercase, ft_strlen(env->hashname_lowercase));
    write(1, ": ", 2);
    write(1, data->payload_name, ft_strlen(data->payload_name));
    write(1, " ", 1);
    write(1, data->err.error_message, ft_strlen(data->err.error_message));
}

static void print_classic_hash_str(t_env *env, t_data *data)
{
    write(1, env->hashname_uppercase, ft_strlen(env->hashname_uppercase));
    write(1, " (", 2);
    write(1, data->payload_name, ft_strlen(data->payload_name));
    write(1, ") = ", 4);
    write(1, data->hash, ft_strlen(data->hash));
}

static void print_flag_r(t_data *data)
{
    write(1, data->hash, ft_strlen(data->hash));
    write(1, " " , 1);
    write(1, data->payload_name, ft_strlen(data->payload_name));
}

static void print_only_hash(t_data *data)
{
    write(1, data->hash, ft_strlen(data->hash));
}

static void print_only_body(t_data *data)
{
    write(1, data->payload, ft_strlen((char*)data->payload));
}

void PrintHashes(t_env *env)
{
    t_data *data = env->data;

    while (data)
    {
        if (!data->err.was_error)
        {
            if (env->flags.p && data->is_stdin) print_only_body(data);

            if (env->flags.q || data->is_stdin) print_only_hash(data);
            else if (env->flags.r) print_flag_r(data);
            else print_classic_hash_str(env, data);
        }
        else print_wrong_command(env, data);

        write(1, "\n", 1);
        data = data->next;
    }
}

BOOL print_and_exit(int error_code, const char *what_called, BOOL return_code)
{
    write(1, "ft_ssl: Error: ", ft_strlen("ft_ssl: Error: "));
    write(1, error_messages[error_code], ft_strlen(error_messages[error_code]));
    write(1, " ", 1);
    write(1, what_called, ft_strlen(what_called));
    write(1, "\n", 1);
    return return_code;
}
